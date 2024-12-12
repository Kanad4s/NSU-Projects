#include "myMutex.h"
#include <stdlib.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sched.h>

static int futex(int* uaddr, int futex_op, int val, const struct timespec* timeout,
          int* uaddr2, int val3) {
  return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

int my_mutex_init(myMutex_t *mutex) {
    mutex->lock = STATUS_UNLOCK;

    return 0;
}

void my_mutex_lock(myMutex_t *mutex) {
    while(1) {
        int expected = STATUS_UNLOCK;
        if(atomic_compare_exchange_weak(&mutex->lock, &expected, STATUS_LOCK)) {
            break;
        }

        int err = futex((int *)&mutex->lock, FUTEX_WAIT, STATUS_LOCK, NULL, NULL, 0);
        if (err == -1) {
            if (errno != EAGAIN) {
                fprintf(stderr, "futex FUTEX_WAIT failed %s\n", strerror(errno));
                abort();
            }
        }
        sched_yield();
    }

    
}

void my_mutex_unlock(myMutex_t *mutex) {
    int expected = STATUS_LOCK;
    atomic_compare_exchange_weak(&mutex->lock, &expected, STATUS_UNLOCK);
    int err = futex((int *)&mutex->lock, FUTEX_WAKE, STATUS_UNLOCK, NULL, NULL, 0);
    if (err == -1) {
            if (errno != EAGAIN) {
                fprintf(stderr, "futex FUTEX_WAKE failed %s\n", strerror(errno));
                abort();
            }
        } 
}
