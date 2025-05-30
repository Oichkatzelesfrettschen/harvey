/*
 * Implementation of the recursive spinlock declared in spinlock.h. The
 * functions below provide basic initialization and locking primitives built on
 * top of C11 atomics.
 */
#include "spinlock.h"
#include <assert.h>

/*
 * Initialize the given Spinlock structure. The lock starts out unlocked and
 * owned by no thread.
 */
void spinlock_init(Spinlock *lock) {
    atomic_flag_clear(&lock->flag);
    lock->owner = 0;
    lock->depth = 0;
}

/*
 * Acquire the lock. If the calling thread already owns the lock, simply
 * increment the recursion depth. Otherwise spin until the lock becomes
 * available.
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

/*
 * Release the lock held by the current thread. If the recursion depth drops to
 * zero, the underlying spin primitive is cleared and other threads may acquire
 * the lock.
 */
void spinlock_release(Spinlock *lock) {
    assert(lock->depth > 0 && thrd_equal(lock->owner, thrd_current()));
    if (--lock->depth == 0) {
        lock->owner = 0;
        atomic_flag_clear_explicit(&lock->flag, memory_order_release);
    }
}
