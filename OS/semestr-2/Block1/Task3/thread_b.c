#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Node {
    int value_int;
    char* ptr_char;
} Node;

void* my_thread(void *arg) {
    // struct Node threadNode;
    // printf("IN THREAD: pid: %d, node: %p\n", getpid(), &threadNode);
    struct Node* node = (struct Node *)arg;
    printf("value_int: %d\n", node->value_int);
    printf("ptr_char: %s\n", node->ptr_char);

    pthread_exit(NULL);
}

int main() {
    struct Node* node = (struct Node *)malloc(sizeof(struct Node));
    // struct Node node;
    node->value_int = 42;
    node->ptr_char = "hello";
    pthread_attr_t attr;
    // printf("pid: %d, node: %p\n", getpid(), &node);

    pthread_attr_init(&attr);
    int err = 0;
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(err) {
        fprintf(stderr, "main: pthread_attr_setdetachstate() failed %s\n", strerror(err));
        return EXIT_FAILURE;
    }

    pthread_t thread;
    err = pthread_create(&thread, &attr, my_thread, (void *)node);
    if (err) {
        fprintf(stderr, "main: pthread_create() failed: %s\n", strerror(err));
        return EXIT_FAILURE;
    }

    sleep(1);
    pthread_attr_destroy(&attr);

    free(node);
    return EXIT_SUCCESS;
}