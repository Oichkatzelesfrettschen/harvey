#include "acd.h"
#include <stdint.h>

Toc thetoc;

int tocthread(void *v) {
    Drive *d;

    d = v;
    LOG(2, "recv ctocdisp?...");
    while (recv(d->ctocdisp, &thetoc) == 1) {
        LOG(2, "recv ctocdisp!...");
        drawtoc(d->w, &thetoc);
        LOG(2, "send dbreq...\n");
        send(d->ctocdbreq, &thetoc);
    }
    return 0;
}

void freetoc(Toc *t) {
    int i;

    free(t->title);
    for (i = 0; i < t->ntrack; i++)
        free(t->track[i].title);
}

int cddbthread(void *v) {
    Drive *d;
    Toc t;

    d = v;
    while (recv(d->ctocdbreply, &t) == 1) {
        if (thetoc.nchange == t.nchange) {
            freetoc(&thetoc);
            thetoc = t;
            redrawtoc(d->w, &thetoc);
        }
    }
    return 0;
}

int cdstatusthread(void *v) {
    Drive *d;
    Cdstatus s;

    d = v;

    for (;;)
        recv(d->cstat, &s);
    return 0;
}
