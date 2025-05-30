#include "acd.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Format an Msf structure as M.S.F for use with Fmt printing routines.
 */
int msfconv(Fmt *fp) {
    Msf m;

    m = va_arg(fp->args, Msf);
    fmtprint(fp, "%d.%d.%d", m.m, m.s, m.f);
    return 0;
}

/* Issue the MMC 'status' command. */
static int status(Drive *d) {
    uint8_t cmd[12];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0xBD;
    return scsi(d->scsi, cmd, sizeof cmd, NULL, 0, Snone);
}

/*
 * Play the CD from the given start frame to end frame using an MMC command.
 */
static int playmsf(Drive *d, Msf start, Msf end) {
    uint8_t cmd[12];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x47;
    cmd[3] = start.m;
    cmd[4] = start.s;
    cmd[5] = start.f;
    cmd[6] = end.m;
    cmd[7] = end.s;
    cmd[8] = end.f;

    return scsi(d->scsi, cmd, sizeof cmd, NULL, 0, Snone);
}

/*
 * Play the range of tracks inclusive using playmsf(). Returns -1 on error.
 */
int playtrack(Drive *d, int start, int end) {
    Toc *t;

    t = &d->toc;

    if (t->ntrack == 0)
        return -1;

    if (start < 0)
        start = 0;
    if (end >= t->ntrack)
        end = t->ntrack - 1;
    if (end < start)
        end = start;

    return playmsf(d, t->track[start].start, t->track[end].end);
}

/* Resume playback after a pause. */
int resume(Drive *d) {
    uint8_t cmd[12];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x4B;
    cmd[8] = 0x01;
    return scsi(d->scsi, cmd, sizeof cmd, NULL, 0, Snone);
}

/* Pause playback. */
int pause(Drive *d) {
    uint8_t cmd[12];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x4B;
    return scsi(d->scsi, cmd, sizeof cmd, NULL, 0, Snone);
}

/* Stop playback. */
int stop(Drive *d) {
    uint8_t cmd[12];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x4E;
    return scsi(d->scsi, cmd, sizeof cmd, NULL, 0, Snone);
}

/* Eject the media from the drive. */
int eject(Drive *d) {
    uint8_t cmd[12];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x1B;
    cmd[1] = 1;
    cmd[4] = 2;
    return scsi(d->scsi, cmd, sizeof cmd, NULL, 0, Snone);
}

/* Load (ingest) media into the drive. */
int ingest(Drive *d) {
    uint8_t cmd[12];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x1B;
    cmd[1] = 1;
    cmd[4] = 3;
    return scsi(d->scsi, cmd, sizeof cmd, NULL, 0, Snone);
}

/* Convert three BCD bytes to an Msf structure. */
static Msf rdmsf(uint8_t *p) {
    Msf msf;

    msf.m = p[0];
    msf.s = p[1];
    msf.f = p[2];
    return msf;
}

/* Read a 24-bit big-endian integer. */
static uint32_t rdlba(uint8_t *p) {
    return (p[0] << 16) | (p[1] << 8) | p[2];
}

/* not a Drive, so that we don't accidentally touch Drive.toc */
/* Query the table of contents from the drive into 't'. */
int gettoc(Scsi *s, Toc *t) {
    int i, n;
    uint8_t cmd[12];
    uint8_t resp[1024];

Again:
    memset(t, 0, sizeof(*t));
    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x43;
    cmd[1] = 0x02;
    cmd[7] = sizeof(resp) >> 8;
    cmd[8] = sizeof(resp);

    s->changetime = 1;
    /* scsi sets nchange, changetime */
    if (scsi(s, cmd, sizeof cmd, resp, sizeof(resp), Sread) < 4)
        return -1;

    if (s->changetime == 0) {
        t->ntrack = 0;
        fprintf(stderr, "no media\n");
        return -1;
    }

    if (t->nchange == s->nchange && t->changetime != 0)
        return 0;

    t->nchange = s->nchange;
    t->changetime = s->changetime;

    if (t->ntrack > MTRACK)
        t->ntrack = MTRACK;

    LOG(2, "%d %d\n", resp[3], resp[2]);
    t->ntrack = resp[3] - resp[2] + 1;
    t->track0 = resp[2];

    n = ((resp[0] << 8) | resp[1]) + 2;
    if (n < 4 + 8 * (t->ntrack + 1)) {
        fprintf(stderr, "bad read0 %d %d: %s\n", n, 4 + 8 * (t->ntrack + 1), strerror(errno));
        return -1;
    }

    for (i = 0; i <= t->ntrack; i++) /* <=: track[ntrack] = end */
        t->track[i].start = rdmsf(resp + 4 + i * 8 + 5);

    for (i = 0; i < t->ntrack; i++)
        t->track[i].end = t->track[i + 1].start;

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x43;
    cmd[7] = sizeof(resp) >> 8;
    cmd[8] = sizeof(resp);
    if (scsi(s, cmd, sizeof cmd, resp, sizeof(resp), Sread) < 4)
        return -1;

    if (s->changetime != t->changetime || s->nchange != t->nchange) {
        fprintf(stderr, "disk changed underfoot; repeating\n");
        goto Again;
    }

    n = ((resp[0] << 8) | resp[1]) + 2;
    if (n < 4 + 8 * (t->ntrack + 1)) {
        fprintf(stderr, "bad read: %s\n", strerror(errno));
        return -1;
    }

    for (i = 0; i <= t->ntrack; i++)
        t->track[i].bstart = rdlba(resp + 4 + i * 8 + 5);

    for (i = 0; i < t->ntrack; i++)
        t->track[i].bend = t->track[i + 1].bstart;

    return 0;
}

/* Debug helper to print the contents of a Toc. */
static void dumptoc(Toc *t) {
    int i;

    fprintf(stdout, "%d tracks\n", t->ntrack);
    for (i = 0; i < t->ntrack; i++)
        print("%d. %M-%M (%lud-%lud)\n", i + 1, t->track[i].start, t->track[i].end,
              t->track[i].bstart, t->track[i].bend);
}

/* Send a benign MMC command to keep the drive awake. */
static void ping(Drive *d) {
    uint8_t cmd[12];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x43;
    scsi(d->scsi, cmd, sizeof(cmd), NULL, 0, Snone);
}

/* Retrieve current play status from the drive. */
static int playstatus(Drive *d, Cdstatus *stat) {
    uint8_t cmd[12], resp[16];

    memset(cmd, 0, sizeof cmd);
    cmd[0] = 0x42;
    cmd[1] = 0x02;
    cmd[2] = 0x40;
    cmd[3] = 0x01;
    cmd[7] = sizeof(resp) >> 8;
    cmd[8] = sizeof(resp);
    if (scsi(d->scsi, cmd, sizeof(cmd), resp, sizeof(resp), Sread) < 0)
        return -1;

    switch (resp[1]) {
    case 0x11:
        stat->state = Splaying;
        break;
    case 0x12:
        stat->state = Spaused;
        break;
    case 0x13:
        stat->state = Scompleted;
        break;
    case 0x14:
        stat->state = Serror;
        break;
    case 0x00: /* not supported */
    case 0x15: /* no current status to return */
    default:
        stat->state = Sunknown;
        break;
    }

    stat->track = resp[6];
    stat->index = resp[7];
    stat->abs = rdmsf(resp + 9);
    stat->rel = rdmsf(resp + 13);
    return 0;
}


/*
 * Background thread that polls the drive for status and TOC changes and sends
 * updates over the drive's channels.
 */
void cdstatusproc(void *v) {
    Drive *d;
    Toc t;
    Cdstatus s;

    t.changetime = ~0;
    t.nchange = ~0;

    d = v;
    LOG(2, "cdstatus %d\n", getpid());
    for (;;) {
        ping(d);
        // LOG(2, "d %d %d t %d %d\n", d->scsi->changetime, d->scsi->nchange, t.changetime,
        // t.nchange);
        if (playstatus(d, &s) == 0)
            send(d->cstatus, &s);
        if (d->scsi->changetime != t.changetime || d->scsi->nchange != t.nchange) {
            if (gettoc(d->scsi, &t) == 0) {
                LOG(2, "sendtoc...\n");
                if (debug)
                    dumptoc(&t);
                send(d->ctocdisp, &t);
            } else
                LOG(2, "error: %r\n");
        }
        sleep(1000);
    }
    return 0;
}
