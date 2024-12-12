#ifndef __MY_SPINLOCK_H__
#define __MY_SPINLOCK_H__

#define _GNU_SOURCE
#include <stdatomic.h>

#define STATUS_LOCK 1
#define STATUS_UNLOCK 0

typedef struct _MySpinLock {
    atomic_int lock;
} mySpinLock_t;

int my_spin_init(mySpinLock_t *spin);
void my_spin_lock(mySpinLock_t *spin);
void my_spin_unlock(mySpinLock_t *spin);
#endif		// __MY_SPINLOCK_H__