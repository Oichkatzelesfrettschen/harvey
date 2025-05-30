#pragma once

/*
 * C17 header for the acd utility.  The original Plan9 dependencies
 * have been removed and replaced with forward declarations and
 * standard C headers so the code can be compiled as freestanding
 * C17.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>

/* Forward declarations for Plan9 types that the old code relied on. */
typedef struct Biobuf Biobuf;
typedef struct Channel Channel;
typedef struct Scsi Scsi;

/*
 * A lightweight approximation of the Plan 9 `Channel`.  Messages are
 * copied into an internal queue protected by a mutex.  When
 * `capacity` is zero the channel behaves synchronously: a sender will
 * block until a receiver consumes the message.
 */
struct Channel {
    mtx_t lock;    /* protects all fields below */
    cnd_t send_cv; /* wait here when no space is available */
    cnd_t recv_cv; /* wait here when no data is available */

    unsigned char *data; /* circular queue storage */
    size_t esize;        /* size of each message */
    int capacity;        /* number of queued messages; 0 => synchronous */
    int count;           /* current number of messages */
    int rpos;            /* read position in `data` */
    int wpos;            /* write position in `data` */
    int waiting_recv;    /* receivers waiting in synchronous mode */
};

typedef struct Alt Alt;
struct Alt {
    Channel *c; /* channel to operate on */
    void *v;    /* buffer for send/recv */
    int op;     /* CHANSND, CHANRCV, ... */
};

enum { CHANSND, CHANRCV, CHANNOP, CHANEND };

Alt mkalt(Channel *, void *, int);
int alt(Alt *);
Channel *chancreate(size_t, int);
int send(Channel *, void *);
int recv(Channel *, void *);
int sendp(Channel *, void *);
void *recvp(Channel *);
int chan_try_send(Channel *, void *);
int chan_try_recv(Channel *, void *);

/* Simple stand-ins for historical Plan9 types. */
typedef unsigned long ulong;
typedef uint32_t uint;

/* acme */
typedef struct Fmt Fmt; /* Formatting context placeholder */
typedef uint32_t Rune;  /* UTF-8 rune */

typedef struct Event Event;
typedef struct Window Window;

enum {
    STACK = 16384,
    EVENTSIZE = 256,
    NEVENT = 5,
};

struct Event {
    int c1;
    int c2;
    int q0;
    int q1;
    int flag;
    int nb;
    int nr;
    char b[EVENTSIZE * 4 + 1]; /* UTF-8 bytes */
    Rune r[EVENTSIZE + 1];     /* Rune data */
};

struct Window {
    /* file descriptors */
    int ctl;
    int event;
    int addr;
    int data;
    Biobuf *body;

    /* event input */
    char buf[512];
    char *bufp;
    int nbuf;
    Event e[NEVENT];

    int id;
    int open;
    Channel *cevent; /* chan(Event*) */
};

extern Window *newwindow(void);
extern int winopenfile(Window *, char *);
extern void winopenbody(Window *, int);
extern void winclosebody(Window *);
extern void wintagwrite(Window *, char *, int);
extern void winname(Window *, char *);
extern void winwriteevent(Window *, Event *);
extern void winread(Window *, uint, uint, char *);
extern int windel(Window *, int);
extern void wingetevent(Window *, Event *);
extern int wineventproc(void *);
extern void winwritebody(Window *, char *, int);
extern void winclean(Window *);
extern int winselect(Window *, char *, int);
extern int winsetaddr(Window *, char *, int);
extern char *winreadbody(Window *, int *);
extern void windormant(Window *);
extern void winsetdump(Window *, char *, char *);

extern char *readfile(char *, char *, int *);
extern void ctlprint(int, char *, ...);
extern void *emalloc(uint);
extern char *estrdup(char *);
extern char *estrstrdup(char *, char *);
extern char *egrow(char *, char *, char *);
extern char *eappend(char *, char *, char *);
extern void error(char *, ...);
extern int tokenizec(char *, char **, int, char *);

/* cd stuff */
typedef struct Msf Msf; /* minute, second, frame */
struct Msf {
    int m;
    int s;
    int f;
};

typedef struct Track Track;
struct Track {
    Msf start;
    Msf end;
    ulong bstart;
    ulong bend;
    char *title;
};

enum {
    MTRACK = 64,
};
typedef struct Toc Toc;
struct Toc {
    int ntrack;
    int nchange;
    int changetime;
    int track0;
    Track track[MTRACK];
    char *title;
};

extern int msfconv(Fmt *);

enum { /* state */
       Sunknown,
       Splaying,
       Spaused,
       Scompleted,
       Serror,
};

typedef struct Cdstatus Cdstatus;
struct Cdstatus {
    int state;
    int track;
    int index;
    Msf abs;
    Msf rel;
};

typedef struct Drive Drive;
struct Drive {
    Window *w;
    Channel *cstatus;  /* chan(Cdstatus) */
    Channel *ctocdisp; /* chan(Toc) */
    Channel *cdbreq;   /* chan(Toc) */
    Channel *cdbreply; /* chan(Toc) */
    Scsi *scsi;
    Toc toc;
    Cdstatus status;
};

int gettoc(Scsi *, Toc *);
void drawtoc(Window *, Drive *, Toc *);
void redrawtoc(Window *, Toc *);
int tocproc(void *);      /* Drive* */
int cddbproc(void *);     /* Drive* */
int cdstatusproc(void *); /* Drive* */

extern int debug;

#define DPRINT                                                                                     \
    if (debug)                                                                                     \
    fprint
void acmeevent(Drive *, Window *, Event *);

int playtrack(Drive *, int, int);
int pause(Drive *);
int resume(Drive *);
int stop(Drive *);
int eject(Drive *);
int ingest(Drive *);

int markplay(Window *, ulong);
int setplaytime(Window *, char *);
void advancetrack(Drive *, Window *);
