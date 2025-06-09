#pragma once

#include <stdatomic.h>
#include <threads.h>

/**
 * @brief Simple recursive spinlock.
 *
 * The lock may be acquired multiple times by the owning thread. @c depth
 * tracks recursion count while @c owner identifies the thread currently
 * holding the lock.
 */
typedef struct {
    atomic_flag flag; /**< Underlying spin primitive. */
    thrd_t owner;     /**< Thread currently holding the lock. */
    int depth;        /**< Recursion count. */
} Spinlock;

/**
 * @brief Initialize a spinlock structure to the unlocked state.
 */
void spinlock_init(Spinlock *lock);

/**
 * @brief Acquire the spinlock, blocking until available.
 */
void spinlock_acquire(Spinlock *lock);

/**
 * @brief Release a previously acquired spinlock.
 */
void spinlock_release(Spinlock *lock);
