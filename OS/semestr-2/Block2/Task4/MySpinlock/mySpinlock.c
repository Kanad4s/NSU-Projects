#include "mySpinlock.h"

int my_spin_init(mySpinLock_t *spin) {
    spin->lock = STATUS_UNLOCK;

    return 0;
}

void my_spin_lock(mySpinLock_t *spin) {
    while(1) {
        int expected = STATUS_UNLOCK;
        if(atomic_compare_exchange_weak(&spin->lock, &expected, STATUS_LOCK)) {
            break;
        }
    }
}

void my_spin_unlock(mySpinLock_t *spin) {
    int expected = STATUS_LOCK;
    atomic_compare_exchange_weak(&spin->lock, &expected, STATUS_UNLOCK);
}
