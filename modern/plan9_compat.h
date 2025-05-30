#pragma once

#ifdef USE_POSIX_THREADS
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/*
 * Simple channel implementation using pthread primitives. A single value can
 * be stored in 'buf' and synchronization is handled with a mutex and
 * condition variable.
 */
typedef struct Channel {
    pthread_mutex_t m;
    pthread_cond_t c;
    void *buf;
    size_t size;
    int ready;
} Channel;

typedef struct Alt Alt;
/*
 * Alt structure used by the alt function to wait on multiple channel
 * operations. Each Alt specifies a channel, a value pointer and an operation.
 */
struct Alt {
    Channel *c;
    void *v;
    int op;
};

enum { CHANSND, CHANRCV, CHANNOP, CHANEND };

/* Create a channel object capable of sending values of the given size. */
static inline Channel *chancreate(size_t size, int buffered) {
    Channel *ch = malloc(sizeof(Channel));
    if (!ch)
        return NULL;
    pthread_mutex_init(&ch->m, NULL);
    pthread_cond_init(&ch->c, NULL);
    ch->buf = malloc(size);
    ch->size = size;
    ch->ready = 0;
    (void)buffered;
    return ch;
}

/* Send a value through the channel, blocking any receivers. */
static inline int send(Channel *ch, void *v) {
    pthread_mutex_lock(&ch->m);
    memcpy(ch->buf, v, ch->size);
    ch->ready = 1;
    pthread_cond_signal(&ch->c);
    pthread_mutex_unlock(&ch->m);
    return 1;
}

/* Receive a value from the channel, blocking until one is available. */
static inline int recv(Channel *ch, void *v) {
    pthread_mutex_lock(&ch->m);
    while (!ch->ready)
        pthread_cond_wait(&ch->c, &ch->m);
    memcpy(v, ch->buf, ch->size);
    ch->ready = 0;
    pthread_mutex_unlock(&ch->m);
    return 1;
}

/* Send a pointer through the channel without copying. */
static inline int sendp(Channel *ch, void *v) {
    pthread_mutex_lock(&ch->m);
    ch->buf = v;
    ch->ready = 1;
    pthread_cond_signal(&ch->c);
    pthread_mutex_unlock(&ch->m);
    return 1;
}

/* Receive a pointer previously sent with sendp. */
static inline void *recvp(Channel *ch) {
    pthread_mutex_lock(&ch->m);
    while (!ch->ready)
        pthread_cond_wait(&ch->c, &ch->m);
    void *v = ch->buf;
    ch->ready = 0;
    pthread_mutex_unlock(&ch->m);
    return v;
}

/* Construct an Alt descriptor for use with alt(). */
static inline Alt mkalt(Channel *c, void *v, int op) {
    Alt a;
    a.c = c;
    a.v = v;
    a.op = op;
    return a;
}

/* Wait on multiple channel operations, returning the index selected. */
static inline int alt(Alt *a) {
    for (int i = 0;; ++i) {
        switch (a[i].op) {
        case CHANRCV:
            if (recv(a[i].c, a[i].v))
                return i;
            break;
        case CHANSND:
            if (send(a[i].c, a[i].v)) {
                a[i].op = CHANNOP;
                return i;
            }
            break;
        case CHANEND:
            return -1;
        default:
            break;
        }
    }
}

/* Spawn a detached thread executing 'fn'. */
static inline int proccreate(void (*fn)(void *), void *arg, unsigned stack) {
    pthread_t t;
    (void)stack;
    return pthread_create(&t, NULL, (void *(*)(void *))fn, arg) == 0;
}

/* Set the current thread's name where supported. */
static inline void threadsetname(const char *name) {
#if defined(__linux__)
    pthread_setname_np(pthread_self(), name);
#elif defined(__APPLE__)
    pthread_setname_np(name);
#else
    (void)name;
#endif
}

/* Terminate the entire process, ignoring the message. */
static inline void threadexitsall(const char *msg) {
    (void)msg;
    exit(0);
}

/* Terminate only the calling thread. */
static inline void threadexits(const char *msg) {
    (void)msg;
    pthread_exit(NULL);
}

#define threadmain main

#else
#include <thread.h>
#endif
