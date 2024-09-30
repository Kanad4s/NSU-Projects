#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEMORY_SIZE 1000 + 4 * 24

//size = 24
typedef struct memBlock {
    size_t size;
    struct memBlock* next;
    int isFree;
} memBlock_t;

static void* memory = NULL;
static memBlock_t* head = NULL;

void init() {
    memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    head = (memBlock_t*)memory;
    head->size = MEMORY_SIZE - sizeof(memBlock_t);
    head->next = NULL;
    head->isFree = 1;
}

void* myMalloc(size_t size) {
    printf("Try allocate %ld bytes\n", size);
    memBlock_t* current;
    void* result = NULL;
    if (head == NULL) {
        init();
    }

    current = head;
    while (current) {
        if (current->isFree && current->size >= size) {
            if (current->size > size + sizeof(memBlock_t)) {
                memBlock_t* new_block = (memBlock_t*)((char*)current + sizeof(memBlock_t) + size);
                new_block->size = current->size - size - sizeof(memBlock_t);
                new_block->next = current->next;
                new_block->isFree = 1;
                current->next = new_block;
                current->size = size;
            }
            current->isFree = 0;
            result = (void*)((char*)current + sizeof(memBlock_t));
            break;
        }
        current = current->next;
    }
    if (result != NULL) {
        printf("Allocated %lu bytes\n", size);
    }
    else {
        printf("Allocation failed\n");
    }
    return result;
}

void myFree(void* ptr) {
    memBlock_t* current;
    if (ptr == NULL || head == NULL) {
        return;
    }
    current = (memBlock_t*)((char*)ptr - sizeof(memBlock_t));
    if (current->isFree) {
        return;
    }
    current->isFree = 1;

    // Merge free blocks
    current = head;
    while (current) {
        if (current->isFree && current->next && current->next->isFree) {
            printf("free %ld bytes\n", current->next->size);
            current->size += sizeof(memBlock_t) + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

int main() {
    int* p1 = myMalloc(100);
    memset(p1, 0x00, 100);
    printf("Malloc: p1 %p\n", p1);
    for (int i = 0; i < 25; ++i) {
        p1[i] = i;
        printf("%d ", p1[i]);
    }
    printf("\n");

    void* p2 = myMalloc(200);
    printf("Malloc: p2 %p\n", p2);

    void* p3 = myMalloc(300);
    printf("Malloc: p3 %p\n", p3);

    void* p4 = myMalloc(400);
    printf("Malloc: p4 %p\n", p4);

    /*printf("Before free: p3 %p\n", p3);
    myFree(p3);
    printf("After free: p3 %p\n", p3);
    myFree(p3);
    printf("Free just again p3: %p\n", p3);*/
    printf("allocated 1000\n");
    myFree(p3);
    p3 = myMalloc(250);
    void* p5 = myMalloc(250);
    printf("Malloc: p5 %p\n", p5);
    /*myFree(p3);
    printf("Free after malloc again p3: %p\n", p3);*/

    myFree(p1);
    myFree(p4);
    myFree(p5);

    int* p6 = myMalloc(100);
    for (int i = 0; i < 20; ++i) {
        printf("%d ", p6[i]);
    }
    printf("\n");
    printf("Malloc: p6 %p\n", p6);
    for (int i = 0; i < 20; ++i) {
        p6[i] = 20 - i;
        printf("%d ", p6[i]);
    }
    printf("\n");
    myFree(p6);
    return 0;
}