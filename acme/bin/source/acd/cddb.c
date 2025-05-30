#include "acd.h"
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

/* see CDDBPROTO */
static uint32_t cddb_sum(int n) {
    int ret;
    ret = 0;
    while (n > 0) {
        ret += n % 10;
        n /= 10;
    }
    return ret;
}

static uint32_t diskid(Toc *t) {
    int i, n, tmp;
    Msf *ms, *me;

    n = 0;
    for (i = 0; i < t->ntrack; i++)
        n += cddb_sum(t->track[i].start.m * 60 + t->track[i].start.s);

    ms = &t->track[0].start;
    me = &t->track[t->ntrack].start;
    tmp = (me->m * 60 + me->s) - (ms->m * 60 + ms->s);

    /*
     * the spec says n%0xFF rather than n&0xFF.  it's unclear which is correct.
     * most CDs are in the database under both entries.
     */
    return ((n & 0xFF) << 24 | (tmp << 8) | t->ntrack);
}

static void append(char **d, char *s) {
    char *r;
    if (*d == NULL)
        *d = estrdup(s);
    else {
        r = emalloc(strlen(*d) + strlen(s) + 1);
        strcpy(r, *d);
        strcat(r, s);
        free(*d);
        *d = r;
    }
}

static int cddbfilltoc(Toc *t) {
    int fd;
    int i;
    char *p, *q;
    FILE *bin; /* buffered connection */
    Msf *m;
    char *f[10];
    int nf;
    char *id, *categ;
    char gottrack[MTRACK];
    int gottitle;

    fd = dial("tcp!freedb.freedb.org!888", 0, 0, 0);
    if (fd < 0) {
        fprintf(stderr, "cannot dial: %s\n", strerror(errno));
        return -1;
    }
    /* turn the fd into a FILE for buffered IO */
    bin = fdopen(fd, "r+");
    if (bin == NULL) {
        perror("fdopen");
        close(fd);
        return -1;
    }

    char line[512];
    if (fgets(line, sizeof(line), bin) == NULL || atoi(line) / 100 != 2) {
    died:
        fclose(bin);
        fprintf(stderr, "error talking to server\n");
        if (line[0] != '\0') {
            line[strcspn(line, "\n")] = '\0';
            fprintf(stderr, "server says: %s\n", line);
        }
        return -1;
    }

    fprintf(bin, "cddb hello gre plan9 9cd 1.0\r\n");
    fflush(bin);
    if (fgets(line, sizeof(line), bin) == NULL || atoi(line) / 100 != 2)
        goto died;

    fprintf(bin, "cddb query %8.8lux %d", diskid(t), t->ntrack);
    DPRINT(2, "cddb query %8.8lux %d", diskid(t), t->ntrack);
    for (i = 0; i < t->ntrack; i++) {
        m = &t->track[i].start;
        fprintf(bin, " %d", (m->m * 60 + m->s) * 75 + m->f);
        DPRINT(2, " %d", (m->m * 60 + m->s) * 75 + m->f);
    }
    m = &t->track[t->ntrack - 1].end;
    fprintf(bin, " %d\r\n", m->m * 60 + m->s);
    DPRINT(2, " %d\r\n", m->m * 60 + m->s);

    fflush(bin);
    if (fgets(line, sizeof(line), bin) == NULL || atoi(line) / 100 != 2)
        goto died;
    line[strcspn(line, "\n")] = '\0';
    DPRINT(2, "cddb: %s\n", line);
    nf = tokenize(line, f, nelem(f));
    if (nf < 1)
        goto died;

    switch (atoi(f[0])) {
    case 200: /* exact match */
        if (nf < 3)
            goto died;
        categ = f[1];
        id = f[2];
        break;
    case 211: /* close matches */
        if (fgets(line, sizeof(line), bin) == NULL)
            goto died;
        if (line[0] == '.') /* no close matches? */
            goto died;
        line[strcspn(line, "\n")] = '\0';

        /* accept first match */
        nf = tokenize(line, f, nelem(f));
        if (nf < 2)
            goto died;
        categ = f[0];
        id = f[1];

        /* snarf rest of buffer */
        while (line[0] != '.') {
            if (fgets(line, sizeof(line), bin) == NULL)
                goto died;
            line[strcspn(line, "\n")] = '\0';
            DPRINT(2, "cddb: %s\n", line);
        }
        break;
    case 202: /* no match */
    default:
        goto died;
    }

    /* fetch results for this cd */
    fprintf(bin, "cddb read %s %s\r\n", categ, id);
    fflush(bin);

    memset(gottrack, 0, sizeof(gottrack));
    gottitle = 0;
    do {
        if (fgets(line, sizeof(line), bin) == NULL)
            goto died;
        q = line + strlen(line) - 1;
        while (isspace((unsigned char)*q))
            *q-- = 0;
        DPRINT(2, "cddb %s\n", line);
        if (strncmp(line, "DTITLE=", 7) == 0) {
            if (gottitle)
                append(&t->title, line + 7);
            else
                t->title = estrdup(line + 7);
            gottitle = 1;
        } else if (strncmp(line, "TTITLE", 6) == 0 && isdigit(line[6])) {
            i = atoi(line + 6);
            if (i < t->ntrack) {
                p = line + 6;
                while (isdigit((unsigned char)*p))
                    p++;
                if (*p == '=')
                    p++;

                if (gottrack[i])
                    append(&t->track[i].title, p);
                else
                    t->track[i].title = estrdup(p);
                gottrack[i] = 1;
            }
        }
    } while (*line != '.');

    fprintf(bin, "quit\r\n");
    fclose(bin);

    return 0;
}

void cddbproc(void *v) {
    Drive *d;
    Toc t;

    threadsetname("cddbproc");
    d = v;
    while (recv(d->cdbreq, &t))
        if (cddbfilltoc(&t) == 0)
            send(d->cdbreply, &t);
}
