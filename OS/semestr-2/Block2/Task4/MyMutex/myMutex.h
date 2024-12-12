#ifndef __MY_MUTEX_H__
#define __MY_MUTEX_H__

#define _GNU_SOURCE
#include <stdatomic.h>

#define STATUS_LOCK 1
#define STATUS_UNLOCK 0

typedef struct _MyMutex {
    atomic_int lock;
} myMutex_t;

int my_mutex_init(myMutex_t *mutex);
void my_mutex_lock(myMutex_t *mutex);
void my_mutex_unlock(myMutex_t *mutex);
#endif		// __MY_MUTEX_H__