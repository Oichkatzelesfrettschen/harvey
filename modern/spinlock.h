#pragma once

#include <stdatomic.h>
#include <threads.h>

/*
 * Simple recursive spinlock. The lock can be acquired multiple times by the
 * owning thread. The depth field tracks the recursion count and the owner field
 * stores the thread that currently holds the lock.
 */
typedef struct {
    atomic_flag flag; /* underlying spin primitive */
    thrd_t owner;     /* thread currently holding the lock */
    int depth;        /* recursion count */
} Spinlock;

/* Initialize a Spinlock structure to an unlocked state. */
void spinlock_init(Spinlock *lock);

/* Acquire the spinlock, blocking until it becomes available. */
void spinlock_acquire(Spinlock *lock);

/* Release a previously acquired spinlock. */
void spinlock_release(Spinlock *lock);
