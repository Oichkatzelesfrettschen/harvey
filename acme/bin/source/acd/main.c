#include "acd.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

int debug;

/* Print usage information and exit. */
void usage(void) {
    fprintf(stderr, "usage: acd dev\n");
    threadexitsall("usage");
}

/* Convenience wrapper for building Alt structures. */
Alt mkalt(Channel *c, void *v, int op) {
    Alt a;

    memset(&a, 0, sizeof(a));
    a.c = c;
    a.v = v;
    a.op = op;
    return a;
}

/* Free dynamically allocated strings in a Toc. */
void freetoc(Toc *t) {
    int i;

    free(t->title);
    for (i = 0; i < t->ntrack; i++)
        free(t->track[i].title);
}

/*
 * Main event loop. Waits on window events, CD status updates and TOC changes
 * and dispatches the appropriate handlers.
 */
void eventwatcher(Drive *d) {
    enum { STATUS, WEVENT, TOCDISP, DBREQ, DBREPLY, NALT };
    Alt alts[NALT + 1];
    Toc nt, tdb;
    Event *e;
    Window *w;
    Cdstatus s;
    char buf[40];

    w = d->w;

    alts[STATUS] = mkalt(d->cstatus, &s, CHANRCV);
    alts[WEVENT] = mkalt(w->cevent, &e, CHANRCV);
    alts[TOCDISP] = mkalt(d->ctocdisp, &nt, CHANRCV);
    alts[DBREQ] = mkalt(d->cdbreq, &tdb, CHANNOP);
    alts[DBREPLY] = mkalt(d->cdbreply, &nt, CHANRCV);
    alts[NALT] = mkalt(NULL, NULL, CHANEND);
    for (;;) {
        switch (alt(alts)) {
        case STATUS:
            // LOG(2, "s...");
            d->status = s;
            if (s.state == Scompleted) {
                s.state = Sunknown;
                advancetrack(d, w);
            }
            // DPRINT(2, "status %d %d %d %M %M\n", s.state, s.track, s.index, s.abs, s.rel);
            snprintf(buf, sizeof(buf), "%d:%2.2d", s.rel.m, s.rel.s);
            // LOG(2, "status %d %d %d %M %M\n", s.state, s.track, s.index, s.abs, s.rel);
            sprint(buf, "%d:%2.2d", s.rel.m, s.rel.s);
            setplaytime(w, buf);
            break;
        case WEVENT:
            // LOG(2, "w...");
            acmeevent(d, w, e);
            break;
        case TOCDISP:
            // LOG(2,"td...");
            freetoc(&d->toc);
            d->toc = nt;
            drawtoc(w, d, &d->toc);
            tdb = nt;
            alts[DBREQ].op = CHANSND;
            break;
        case DBREQ: /* sent */
            // LOG(2,"dreq...");
            alts[DBREQ].op = CHANNOP;
            break;
        case DBREPLY:
            // LOG(2,"drep...");
            freetoc(&d->toc);
            d->toc = nt;
            redrawtoc(w, &d->toc);
            break;
        }
    }
}

int main(int argc, char **argv) {
/* Program entry point when using plan9 threads. */
void threadmain(int argc, char **argv) {
    Scsi *s;
    Drive *d;
    char buf[80];

    ARGBEGIN {
    case 'v':
        debug++;
        scsiverbose++;
    }
    ARGEND

    if (argc != 1)
        usage();

    /* randomize alt scheduling */
    srand((unsigned)time(NULL));

    fmtinstall('M', msfconv);

    if ((s = openscsi(argv[0])) == NULL)
        error("opening scsi: %r");

    d = malloc(sizeof(*d));
    if (d == NULL)
        error("out of memory");
    memset(d, 0, sizeof d);

    d->scsi = s;
    d->w = newwindow();
    d->ctocdisp = chancreate(sizeof(Toc), 0);
    d->cdbreq = chancreate(sizeof(Toc), 0);
    d->cdbreply = chancreate(sizeof(Toc), 0);
    d->cstatus = chancreate(sizeof(Cdstatus), 0);

    thrd_t t1, t2, t3;
    thrd_create(&t1, wineventproc, d->w);
    thrd_detach(t1);
    thrd_create(&t2, cddbproc, d);
    thrd_detach(t2);
    thrd_create(&t3, cdstatusproc, d);
    thrd_detach(t3);

    cleanname(argv[0]);
    snprint(buf, sizeof(buf), "%s/", argv[0]);
    winname(d->w, buf);

    wintagwrite(d->w, "Stop Pause Resume Eject Ingest ", 5 + 6 + 7 + 6 + 7);
    eventwatcher(d);
    return 0;
}
