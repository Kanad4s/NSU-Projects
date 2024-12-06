#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_STRING_LENGTH 100
#define STORAGE_SIZE 1000
#define THREADS_COUNT 7

int inc_count = 0;
int inc_iter = 0;

int decr_count = 0;
int decr_iter = 0;

int eq_count = 0;
int eq_iter = 0;

int swap_count = 0;
int swap_iter = 0;

typedef struct _ListNode {
    char val[MAX_STRING_LENGTH];
    struct _ListNode *next;
    pthread_spinlock_t sync;
} node_t;

typedef struct _LinkedList {
    node_t *first;
} linked_list_t;

node_t *create_node(char *val) {
    node_t *node = malloc(sizeof(node_t));

    strncpy(node->val, val, MAX_STRING_LENGTH);
    node->next = NULL;
    pthread_spin_init(&node->sync, PTHREAD_PROCESS_SHARED);

    return node;
}

void linked_list_destroy(linked_list_t *ll) {
    node_t *cur = ll->first;
    while (cur->next != NULL) {
        node_t *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(cur);
}

void *inc_routine(void *args) {
    linked_list_t *ll = (linked_list_t *) args;

    while (true) {
        pthread_spin_lock(&ll->first->sync);
        node_t *prev = ll->first, *cur;
        while (prev->next != NULL) {
            cur = prev->next;
            int prevSize = strlen(prev->val);
            pthread_spin_lock(&cur->sync);
            pthread_spin_unlock(&prev->sync);
            if (prevSize < strlen(cur->val)) {
                inc_count++;
            }
            prev = cur;
        }
        pthread_spin_unlock(&prev->sync);
        ++inc_iter;
    }
}

void *decr_routine(void *args) {
    linked_list_t *ll = (linked_list_t *) args;

    while (true) {
        pthread_spin_lock(&ll->first->sync);
        node_t *prev = ll->first, *cur;
        while (prev->next != NULL) {
            cur = prev->next;
            int prevSize = strlen(prev->val);
            pthread_spin_lock(&cur->sync);
            pthread_spin_unlock(&prev->sync);
            if (prevSize > strlen(cur->val)) {
                decr_count++;
            }
            prev = cur;
        }
        pthread_spin_unlock(&prev->sync);
        ++decr_iter;
    }
}

void *eq_routine(void *args) {
    linked_list_t *ll = (linked_list_t *) args;

    while (true) {
        pthread_spin_lock(&ll->first->sync);
        node_t *prev = ll->first, *cur;
        while (prev->next != NULL) {
            cur = prev->next;
            int prevSize = strlen(prev->val);
            pthread_spin_lock(&cur->sync);
            pthread_spin_unlock(&prev->sync);
            if (prevSize == strlen(cur->val)) {
                eq_count++;
            }
            prev = cur;
        }
        pthread_spin_unlock(&prev->sync);
        ++eq_iter;
    }
}
void *swap_routine(void *args) {
    linked_list_t *ll = (linked_list_t *) args;

    while (true) {
        pthread_spin_lock(&ll->first->sync);
        node_t *prev = ll->first, *cur, *next;
        while (prev->next != NULL) {
            if (rand() % 100 != 0) {
                cur = prev->next;
                pthread_spin_lock(&cur->sync);
                pthread_spin_unlock(&prev->sync);
                prev = cur;
                continue;
            }
            cur = prev->next;
            pthread_spin_lock(&cur->sync);
            next = cur->next;
            if (next == NULL) {
                pthread_spin_unlock(&cur->sync);
                break;
            }
            pthread_spin_lock(&next->sync);
            prev->next = next;
            pthread_spin_unlock(&prev->sync);
            cur->next = next->next;
            pthread_spin_unlock(&cur->sync);
            next->next = cur;
            swap_count++;
            prev = next;
        }
        pthread_spin_unlock(&prev->sync);
        ++swap_iter;
    }
}

void *print_routine(void *args) {
    while (true) {
        sleep(1);
        printf("inc: %d/%d,\t\tdecr: %d/%d,\t\teq: %d/%d,\t\tswap: %d/%d\n", inc_count, inc_iter, decr_count, decr_iter,
               eq_count, eq_iter, swap_count, swap_iter);
    }
}

int main() {
    linked_list_t *ll = malloc(sizeof(linked_list_t));

    srand(time(NULL));
    char val[MAX_STRING_LENGTH];
    memset(val, 0, MAX_STRING_LENGTH);
    memset(val, '0', rand() % MAX_STRING_LENGTH);
    ll->first = create_node(val);

    node_t *last = ll->first;
    for (int i = 1; i < STORAGE_SIZE; ++i) {
        memset(val, 0, MAX_STRING_LENGTH);
        memset(val, '0', rand() % MAX_STRING_LENGTH);
        node_t *newNode = create_node(val);
        last->next = newNode;
        last = newNode;
    }

    pthread_t tid[THREADS_COUNT];

    int err = pthread_create(&tid[0], NULL, inc_routine, ll);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[1], NULL, decr_routine, ll);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[2], NULL, eq_routine, ll);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[3], NULL, print_routine, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }

    for (int i = 0; i < 3; ++i) {
        err = pthread_create(&tid[4 + i], NULL, swap_routine, ll);
        if (err) {
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }

    sleep(10);

    for (int i = 0; i < THREADS_COUNT; ++i) {
        pthread_cancel(tid[i]);
    }

    linked_list_destroy(ll);
    free(ll);
}