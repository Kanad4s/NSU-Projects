#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <sys/types.h>

#define PAGE 4096
#define STACK_SIZE PAGE*8

typedef void *(*start_routine_t) (void *);

typedef struct _mythread {
    int mythread_id;
    void *arg;
    start_routine_t start_routine;
    void *retval;
    volatile int finished;
    volatile int joined;
} mythread_struct_t;

typedef mythread_struct_t* mythread_t;

int mythread_create(mythread_t *mytid, void *(*start_routine) (void *), void* arg);
void mythread_join(mythread_t mytid, void **retval);

#endif // MYTHREAD_H
