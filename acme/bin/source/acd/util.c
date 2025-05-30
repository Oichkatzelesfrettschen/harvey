#include "acd.h"
#include <stdint.h>
#include <stdlib.h>

/* Allocate zero-initialized memory or abort on failure. */
void *emalloc(uint n) {
    void *p;

    p = malloc(n);
    if (p == NULL)
        error("can't malloc: %r");
    memset(p, 0, n);
    return p;
}

/* Duplicate a string using emalloc. */
char *estrdup(char *s) {
    char *t;

    t = emalloc(strlen(s) + 1);
    strcpy(t, s);
    return t;
}

/* Duplicate two strings into newly allocated memory. */
char *estrstrdup(char *s, char *t) {
    char *u;

    u = emalloc(strlen(s) + strlen(t) + 1);
    strcpy(u, s);
    strcat(u, t);
    return u;
}

/* Append string 't' to 's' with a separator. */
char *eappend(char *s, char *sep, char *t) {
    char *u;

    if (t == NULL)
        u = estrstrdup(s, sep);
    else {
        u = emalloc(strlen(s) + strlen(sep) + strlen(t) + 1);
        strcpy(u, s);
        strcat(u, sep);
        strcat(u, t);
    }
    free(s);
    return u;
}

/* Append 't' to 's' and free the appended piece. */
char *egrow(char *s, char *sep, char *t) {
    s = eappend(s, sep, t);
    free(t);
    return s;
}

/* Print an error message and terminate all threads. */
void error(char *fmt, ...) {
    int n;
    va_list arg;
    char buf[256];

    fprintf(stderr, "Mail: ");
    va_start(arg, fmt);
    n = vsnprint(buf, sizeof buf, fmt, arg);
    va_end(arg);
    write(2, buf, n);
    write(2, "\n", 1);
    exit(1);
}

/* Formatted write to a control file, aborting on failure. */
void ctlprint(int fd, char *fmt, ...) {
    int n;
    va_list arg;
    char buf[256];

    va_start(arg, fmt);
    n = vsnprint(buf, sizeof buf, fmt, arg);
    va_end(arg);
    if (write(fd, buf, n) != n)
        error("control file write error: %r");
}

/*
 * Channel primitives ---------------------------------------------------------
 */

/*
 * Allocate a new channel capable of storing up to `buffered` messages of
 * `size` bytes. When `buffered` is zero the channel acts synchronously:
 * senders wait for receivers to accept the value.
 */
Channel *chancreate(size_t size, int buffered) {
    Channel *c = emalloc(sizeof(Channel));

    mtx_init(&c->lock, mtx_plain);
    cnd_init(&c->send_cv);
    cnd_init(&c->recv_cv);

    c->esize = size;
    c->capacity = buffered;
    if (c->capacity == 0)
        c->capacity = 1; /* storage for rendezvous */
    c->data = emalloc(c->esize * c->capacity);

    c->count = 0;
    c->rpos = 0;
    c->wpos = 0;
    c->waiting_recv = 0;

    return c;
}

/* internal helper implementing blocking/non-blocking send */
static int chan_send_common(Channel *c, void *v, int block) {
    mtx_lock(&c->lock);

    while (c->capacity == 1 && c->waiting_recv == 0)
        cnd_wait(&c->send_cv, &c->lock);

    while (c->capacity > 0 && c->count == c->capacity) {
        if (!block) {
            mtx_unlock(&c->lock);
            return 0;
        }
        cnd_wait(&c->send_cv, &c->lock);
    }

    memcpy(c->data + c->wpos * c->esize, v, c->esize);
    c->wpos = (c->wpos + 1) % c->capacity;
    c->count++;
    cnd_signal(&c->recv_cv);

    if (c->capacity == 1) {
        while (c->count == 1 && block)
            cnd_wait(&c->send_cv, &c->lock);
        if (c->count == 1 && !block) {
            mtx_unlock(&c->lock);
            return 0;
        }
    }

    mtx_unlock(&c->lock);
    return 1;
}

/* internal helper implementing blocking/non-blocking receive */
static int chan_recv_common(Channel *c, void *v, int block) {
    mtx_lock(&c->lock);

    c->waiting_recv++;
    cnd_signal(&c->send_cv);

    while (c->count == 0) {
        if (!block) {
            c->waiting_recv--;
            mtx_unlock(&c->lock);
            return 0;
        }
        cnd_wait(&c->recv_cv, &c->lock);
    }

    memcpy(v, c->data + c->rpos * c->esize, c->esize);
    c->rpos = (c->rpos + 1) % c->capacity;
    c->count--;
    c->waiting_recv--;
    cnd_signal(&c->send_cv);

    mtx_unlock(&c->lock);
    return 1;
}

int send(Channel *c, void *v) {
    return chan_send_common(c, v, 1);
}
int recv(Channel *c, void *v) {
    return chan_recv_common(c, v, 1);
}
int chan_try_send(Channel *c, void *v) {
    return chan_send_common(c, v, 0);
}
int chan_try_recv(Channel *c, void *v) {
    return chan_recv_common(c, v, 0);
}

int sendp(Channel *c, void *v) {
    return send(c, &v);
}

void *recvp(Channel *c) {
    void *v = NULL;
    recv(c, &v);
    return v;
}

Alt mkalt(Channel *c, void *v, int op) {
    Alt a = {c, v, op};
    return a;
}

/*
 * Simple alt implementation that repeatedly attempts the operations
 * without blocking until one succeeds.  This is sufficient for the
 * narrow use within eventwatcher.
 */
int alt(Alt *a) {
    /* count choices */
    int n = 0;
    while (a[n].op != CHANEND)
        n++;

    for (;;) {
        int start = rand() % n;
        for (int off = 0; off < n; off++) {
            int i = (start + off) % n;
            switch (a[i].op) {
            case CHANSND:
                if (chan_try_send(a[i].c, a[i].v)) {
                    a[i].op = CHANNOP;
                    return i;
                }
                break;
            case CHANRCV:
                if (chan_try_recv(a[i].c, a[i].v))
                    return i;
                break;
            default:
                break;
            }
        }
        thrd_yield();
    }
}
