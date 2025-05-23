#include "spinlock.h"
#include <assert.h>

void spinlock_init(Spinlock *lock) {
    atomic_flag_clear(&lock->flag);
    lock->owner = 0;
    lock->depth = 0;
}

void spinlock_acquire(Spinlock *lock) {
    thrd_t self = thrd_current();
    if (lock->depth && thrd_equal(lock->owner, self)) {
        lock->depth++;
        return;
    }
    while (atomic_flag_test_and_set_explicit(&lock->flag, memory_order_acquire)) {
        ;
    }
    lock->owner = self;
    lock->depth = 1;
}

void spinlock_release(Spinlock *lock) {
    assert(lock->depth > 0 && thrd_equal(lock->owner, thrd_current()));
    if (--lock->depth == 0) {
        lock->owner = 0;
        atomic_flag_clear_explicit(&lock->flag, memory_order_release);
    }
}
