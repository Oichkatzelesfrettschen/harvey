/**
 * @file spinlock.c
 * @brief Implementation of the recursive spinlock declared in spinlock.h.
 */
#include "spinlock.h"
#include <assert.h>

/**
 * @brief Initialize the given spinlock.
 *
 * The lock starts out unlocked and owned by no thread.
 */
void spinlock_init(Spinlock *lock) {
    atomic_flag_clear(&lock->flag);
    lock->owner = 0;
    lock->depth = 0;
}

/**
 * @brief Acquire the lock.
 *
 * If the calling thread already owns the lock the recursion depth is
 * incremented. Otherwise the function spins until the lock becomes available.
 */
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

/**
 * @brief Release the lock held by the current thread.
 *
 * When the recursion depth drops to zero the underlying spin primitive is
 * cleared so other threads may acquire the lock.
 */
void spinlock_release(Spinlock *lock) {
    assert(lock->depth > 0 && thrd_equal(lock->owner, thrd_current()));
    if (--lock->depth == 0) {
        lock->owner = 0;
        atomic_flag_clear_explicit(&lock->flag, memory_order_release);
    }
}
