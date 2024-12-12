#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define ITER_COUNT 100000000

volatile long int counter = 0;

void set_cpu(int n) {
	int err;
	cpu_set_t cpuset;
	pthread_t tid = pthread_self();

	CPU_ZERO(&cpuset);
	CPU_SET(n, &cpuset);

	err = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
	if (err) {
		printf("set_cpu: pthread_setaffinity failed for cpu %d\n", n);
		return;
	}

	printf("set_cpu: set cpu %d\n", n);
}

void incr_var() {
    for (int i = 0; i < ITER_COUNT; i++) {
        counter++;
    }
}

void *mythread1(void *arg) {
    char *str = (char *)arg;

    printf("mythread1: args: '%s'. pid %d; ppid %d; tid %d\n", str, getpid(), getppid(), gettid());

    set_cpu(1);

    incr_var();

    return NULL;
}

void *mythread2(void *arg) {
    char *str = (char *)arg;

    printf("mythread2: args: '%s'. pid %d; ppid %d; tid %d\n", str, getpid(), getppid(), gettid());

    set_cpu(2);

    for (int i = 0; i < ITER_COUNT; i++) {
        counter++;
    }

    return NULL;
}

int main() {
    int err;
    char arg[16];
    pthread_t tid1, tid2;

    snprintf(arg, sizeof(arg), "thread 1");
    err = pthread_create(&tid1, NULL, mythread1,( void *)arg);
    if (err) {
        printf("main: 1 pthread_create() failed: %s\n", strerror(err));
        return -1;
    }

    snprintf(arg, sizeof(arg), "thread 2");
    err = pthread_create(&tid2, NULL, mythread2,( void *)arg);
    if (err) {
        printf("main: 1 pthread_create() failed: %s\n", strerror(err));
        return -1;
    }

    err = pthread_join(tid2, NULL);
    if (err) {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }

    err = pthread_join(tid1, NULL);
    if (err) {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }

    printf("main: counter = %ld\n", counter);

    if (counter != 2 * ITER_COUNT) {
        return 1;
    }

    return 0;
}