#include "acd.h"
#include <errno.h>
#include <stdint.h>

/*
 * Allocate and initialize a new acme window by interacting with
 * the /mnt/wsys control files.  Returns a freshly allocated Window
 * structure on success and terminates the program on failure.
 */
Window *newwindow(void) {
    char buf[12];
    Window *w;

    w = emalloc(sizeof(Window));
    w->ctl = open("/mnt/wsys/new/ctl", ORDWR | OCEXEC);
    if (w->ctl < 0 || read(w->ctl, buf, 12) != 12)
        error("can't open window ctl file: %s", strerror(errno));
    ctlprint(w->ctl, "noscroll\n");
    w->id = atoi(buf);
    w->event = winopenfile(w, "event");
    w->addr = -1; /* will be opened when needed */
    w->body = NULL;
    w->data = -1;
    w->cevent = chancreate(sizeof(Event *), 0);
    if (w->cevent == NULL)
        error("cevent allocation failed");
    return w;
}

/* Configure dump directory and command for the window. */
void winsetdump(Window *w, char *dir, char *cmd) {
    if (dir != NULL)
        ctlprint(w->ctl, "dumpdir %s\n", dir);
    if (cmd != NULL)
        ctlprint(w->ctl, "dump %s\n", cmd);
}

/*
 * Background thread that continuously reads events from the window
 * and forwards them over the channel attached to the Window.
 */
int wineventproc(void *v) {
    Window *w;
    int i;

    w = v;
    for (i = 0;; i++) {
        if (i >= NEVENT)
            i = 0;
        wingetevent(w, &w->e[i]);
        sendp(w->cevent, &w->e[i]);
    }
    return 0;
}

/*
 * Open one of the per-window files under /mnt/wsys/<id>/ using
 * regular POSIX I/O and return its file descriptor.
 */
int winopenfile(Window *w, char *f) {
    char buf[64];
    int fd;

    snprintf(buf, sizeof(buf), "/mnt/wsys/%d/%s", w->id, f);
    fd = open(buf, ORDWR | OCEXEC);
    if (fd < 0)
        error("can't open window file %s: %s", f, strerror(errno));
    return fd;
}

/* Write a string to the window's tag file. */
void wintagwrite(Window *w, char *s, int n) {
    int fd;

    fd = winopenfile(w, "tag");
    if (write(fd, s, n) != n)
        error("tag write: %s", strerror(errno));
    close(fd);
}

/* Set the window's name string. */
void winname(Window *w, char *s) {
    ctlprint(w->ctl, "name %s\n", s);
}

/*
 * Open the body file associated with the window.  The file is opened
 * using stdio so reads and writes can be buffered conveniently.
 */
void winopenbody(Window *w, int mode) {
    char buf[256];

    /* open the body file using stdio for buffered access */
    snprintf(buf, sizeof(buf), "/mnt/wsys/%d/body", w->id);
    w->body = fopen(buf, mode == OREAD ? "r" : "w");
    if (w->body == NULL)
        error("can't open window body file: %s", strerror(errno));
}

/* Close the buffered body file if it is open. */
void winclosebody(Window *w) {
    if (w->body != NULL) {
        /* close and discard buffered body file */
        fclose(w->body);
        w->body = NULL;
    }
}

/* Append text to the window body using buffered I/O. */
void winwritebody(Window *w, char *s, int n) {
    if (w->body == NULL)
        winopenbody(w, OWRITE);
    /* write buffered body text */
    if (fwrite(s, 1, n, w->body) != (size_t)n)
        error("write error to window: %s", strerror(errno));
}

int wingetec(Window *w) {
    if (w->nbuf == 0) {
        w->nbuf = read(w->event, w->buf, sizeof w->buf);
        if (w->nbuf <= 0) {
            /* probably because window has exited, and only called by wineventproc, so just shut
             * down */
            thrd_exit(NULL);
        }
        w->bufp = w->buf;
    }
    w->nbuf--;
    return *w->bufp++;
}

int wingeten(Window *w) {
    int n, c;

    n = 0;
    while ('0' <= (c = wingetec(w)) && c <= '9')
        n = n * 10 + (c - '0');
    if (c != ' ')
        error("event number syntax");
    return n;
}

int wingeter(Window *w, char *buf, int *nb) {
    Rune r;
    int n;

    r = wingetec(w);
    buf[0] = r;
    n = 1;
    if (r >= Runeself) {
        while (!fullrune(buf, n))
            buf[n++] = wingetec(w);
        chartorune(&r, buf);
    }
    *nb = n;
    return r;
}

void wingetevent(Window *w, Event *e) {
    int i, nb;

    e->c1 = wingetec(w);
    e->c2 = wingetec(w);
    e->q0 = wingeten(w);
    e->q1 = wingeten(w);
    e->flag = wingeten(w);
    e->nr = wingeten(w);
    if (e->nr > EVENTSIZE)
        error("event string too long");
    e->nb = 0;
    for (i = 0; i < e->nr; i++) {
        e->r[i] = wingeter(w, e->b + e->nb, &nb);
        e->nb += nb;
    }
    e->r[e->nr] = 0;
    e->b[e->nb] = 0;
    if (wingetec(w) != '\n')
        error("event syntax error");
}

void winwriteevent(Window *w, Event *e) {
    /* send event using stdio wrappers */
    FILE *fp = fdopen(dup(w->event), "w");
    if (fp) {
        fprintf(fp, "%c%c%d %d\n", e->c1, e->c2, e->q0, e->q1);
        fclose(fp);
    }
}

static int nrunes(char *s, int nb) {
    int i, n;
    Rune r;

    n = 0;
    for (i = 0; i < nb; n++)
        i += chartorune(&r, s + i);
    return n;
}

void winread(Window *w, uint q0, uint q1, char *data) {
    int m, n, nr;
    char buf[256];

    if (w->addr < 0)
        w->addr = winopenfile(w, "addr");
    if (w->data < 0)
        w->data = winopenfile(w, "data");
    m = q0;
    while (m < q1) {
        n = snprintf(buf, sizeof(buf), "#%d", m);
        if (write(w->addr, buf, n) != n)
            error("error writing addr: %s", strerror(errno));
        n = read(w->data, buf, sizeof buf);
        if (n <= 0)
            error("reading data: %s", strerror(errno));
        nr = nrunes(buf, n);
        while (m + nr > q1) {
            do
                ;
            while (n > 0 && (buf[--n] & 0xC0) == 0x80);
            --nr;
        }
        if (n == 0)
            break;
        memmove(data, buf, n);
        data += n;
        *data = 0;
        m += nr;
    }
}

/* Close any open per-window resources without deleting the window. */
void windormant(Window *w) {
    if (w->addr >= 0) {
        close(w->addr);
        w->addr = -1;
    }
    if (w->body != NULL) {
        /* close buffered body handle */
        fclose(w->body);
        w->body = NULL;
    }
    if (w->data >= 0) {
        close(w->data);
        w->data = -1;
    }
}

/* Delete the window.  If 'sure' is non-zero the delete is forced. */
int windel(Window *w, int sure) {
    if (sure)
        write(w->ctl, "delete\n", 7);
    else if (write(w->ctl, "del\n", 4) != 4)
        return 0;
    /* event proc will die due to read error from event file */
    windormant(w);
    close(w->ctl);
    w->ctl = -1;
    close(w->event);
    w->event = -1;
    return 1;
}

/* Flush pending body writes and mark the window clean. */
void winclean(Window *w) {
    if (w->body)
        /* flush buffered writes */
        fflush(w->body);
    ctlprint(w->ctl, "clean\n");
}

int winsetaddr(Window *w, char *addr, int errok) {
    if (w->addr < 0)
        w->addr = winopenfile(w, "addr");
    if (write(w->addr, addr, strlen(addr)) < 0) {
        if (!errok)
            error("error writing addr(%s): %s", addr, strerror(errno));
        return 0;
    }
    return 1;
}

int winselect(Window *w, char *addr, int errok) {
    if (winsetaddr(w, addr, errok)) {
        ctlprint(w->ctl, "dot=addr\n");
        return 1;
    }
    return 0;
}

/*
 * Read the entire body of the window into a newly allocated buffer.
 * The caller must free the returned string.  The number of bytes
 * read is stored in *np.
 */
char *winreadbody(Window *w, int *np) {
    char *s;
    int m, na, n;

    if (w->body != NULL)
        winclosebody(w);
    winopenbody(w, OREAD);
    s = NULL;
    na = 0;
    n = 0;
    for (;;) {
        if (na < n + 512) {
            na += 1024;
            s = realloc(s, na + 1);
        }
        /* read from buffered body */
        m = fread(s + n, 1, na - n, w->body);
        if (m <= 0)
            break;
        n += m;
    }
    s[n] = 0;
    winclosebody(w);
    *np = n;
    return s;
}
