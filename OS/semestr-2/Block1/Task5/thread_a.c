#define _GNU_SOURCE
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int threadsCount = 3;

sigset_t allSignalsMask;

void sigHandler(int signo) {
  printf("Caught signal %d, tid %d\n", signo, gettid());
}

void* blockAllSignals() {
    printf("\nSTART blockAllSignals: %d\n", gettid());
    sigset_t oldSet;

    int err = pthread_sigmask(SIG_BLOCK, &allSignalsMask, &oldSet);
    if (err){
        perror("pthread_sigmask() ");
    }

    sleep(10);
    return NULL;
}

void* handleSigIntThread() {
    printf("\nStart handleSigintThread: %d\n", gettid());
    signal(SIGINT, sigHandler);

    sleep(10);
    return NULL;
}

void* handleSigQuitThread() {
    int sig;
    printf("\nSTART handleSigQuitThread: %d\n", gettid());
    signal(SIGQUIT, sigHandler);
    sigset_t sigSet, oldSet;
    sigemptyset(&sigSet);
    pthread_sigmask(SIG_BLOCK, &sigSet, &oldSet);

    int ret = sigwait(&allSignalsMask, &sig);
    if(ret) {
        return NULL;
    }

    printf("receive sig %d\n", sig);
    sleep(3);
    return NULL;
}

int main() {
    void* ret_val;
    pthread_t tid[threadsCount];
    int err = sigfillset(&allSignalsMask);

    err = pthread_create(&tid[0], NULL, blockAllSignals, NULL);
    if (err) {
        fprintf(stderr, "blockAllSignals(): %s\n", strerror(err));
    }
    sleep(3);
    pthread_kill(tid[0], SIGINT);

    err = pthread_create(&tid[1], NULL, handleSigIntThread, NULL);
    if (err) {
        fprintf(stderr, "handleSigIntThread(): %s\n", strerror(err));
    }
    sleep(3);
    pthread_kill(tid[1], SIGINT);

    err = pthread_create(&tid[2], NULL, handleSigQuitThread, NULL);
    if (err) {
        fprintf(stderr, "handleSigQuitThread(): %s\n", strerror(err));
    }
    sleep(3);

    for(int i = 0; i < threadsCount; ++i) {
        err = pthread_join(tid[i], &ret_val);
        if (err) {
            fprintf(stderr, "main: pthread_join() failed %s\n", strerror(err));
        }
    }
    printf("live main thread");
    sleep(3);
    return EXIT_SUCCESS;
}