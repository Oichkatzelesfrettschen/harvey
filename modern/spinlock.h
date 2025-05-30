#pragma once

#include <stdatomic.h>
#include <threads.h>

/* Recursive spinlock that can be acquired multiple times by the same thread. */
typedef struct {
    atomic_flag flag;
    thrd_t owner;
    int depth;
} Spinlock;

void spinlock_init(Spinlock *lock);
void spinlock_acquire(Spinlock *lock);
void spinlock_release(Spinlock *lock);
