#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PAGE_SIZE 0x1000
#define NAME_SIZE 256
#define OFFSET_SIZE 12

unsigned long long getOffset(char fileName[]) {
    char* maps = (char *)malloc(NAME_SIZE * sizeof(char));
    strcat(maps, fileName);
    strcat(maps, "/maps");
    char* offsetLine = (char*)malloc(OFFSET_SIZE * sizeof(char));
    int fd = open(maps, O_RDONLY); // O_RDONLY means read only
    if (fd < 0) {
        perror("Error openning /maps");
    }
    if (pread(fd, offsetLine, OFFSET_SIZE, 0) != OFFSET_SIZE) {
        perror("Error reading offset");
    }
    char* endPtr;
    //printf("%llu", strtoull(offsetLine, &endPtr, 16));
    close(fd);
    free(maps);
    return strtoull(offsetLine, &endPtr, 16);
}

int hasInformation(unsigned long long data) {
    if (((data >> 55) & 1) || ((data >> 56) & 1) || ((data >> 61) & 1) || ((data >> 62) & 1) || ((data >> 63) & 1)) {
        return 1;
    }
    return 0;
}

void printPageInfo(unsigned long long address, unsigned long long data) {
    printf("virtual address: 0x%-16llx, soft-dirty: %lld, exclusively mapped: %lld, file page/shared: %lld, swapped: %lld, present: %lld\n",
        address, (data >> 55) & 1, (data >> 56) & 1, (data >> 61) & 1, (data >> 62) & 1, (data >> 63) & 1);
}

void printMap(int argc, char fileName[]) {
    char* pagemap = (char*)malloc(NAME_SIZE * sizeof(char));
    strcat(pagemap, fileName);
    strcat(pagemap, "/pagemap");
    int fd = open(pagemap, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open pagemap file");
        close(fd);
        return;
    }

    unsigned long long offset = getOffset(fileName);

    for (unsigned long long i = offset; i < 0xffffffffffff; i += PAGE_SIZE) {
        unsigned long long data;
        unsigned long long shift = (i / PAGE_SIZE) * sizeof(data);
        if (pread(fd, &data, sizeof(data), shift) != sizeof(data)) {
            perror("Error reading pagemap");
            close(fd);
            return;
        }
        if (hasInformation(data)) {
            printPageInfo(i, data);
        }
        else {
            printf("virtual address: 0x%-16llx, data:%llu\n", i, data);
        }
        usleep(1000000);
    }
    close(fd);
    free(pagemap);
}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        printf("program does not have arguments\n");
        return 0;
    }
    char filename[NAME_SIZE] = "/proc/self";
    printMap(argc, filename);
    return 0;
}
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PAGE_SIZE 0x1000
#define NAME_SIZE 256
#define OFFSET_SIZE 12

unsigned long long getOffset(char fileName[]) {
    char* maps = (char *)malloc(NAME_SIZE * sizeof(char));
    strcat(maps, fileName);
    strcat(maps, "/maps");
    char* offsetLine = (char*)malloc(OFFSET_SIZE * sizeof(char));
    int fd = open(maps, O_RDONLY); // O_RDONLY means read only
    if (fd < 0) {
        perror("Error openning /maps");
    }
    if (pread(fd, offsetLine, OFFSET_SIZE, 0) != OFFSET_SIZE) {
        perror("Error reading offset");
    }
    char* endPtr;
    //printf("%llu", strtoull(offsetLine, &endPtr, 16));
    close(fd);
    free(maps);
    return strtoull(offsetLine, &endPtr, 16);
}

int hasInformation(unsigned long long data) {
    if (((data >> 55) & 1) || ((data >> 56) & 1) || ((data >> 61) & 1) || ((data >> 62) & 1) || ((data >> 63) & 1)) {
        return 1;
    }
    return 0;
}

void printPageInfo(unsigned long long address, unsigned long long data) {
    printf("virtual address: 0x%-12llx, data:0x%-12llx, soft-dirty: %lld, exclusively mapped: %lld, file page/shared: %lld, swapped: %lld, present: %lld\n",
        address, data, (data >> 55) & 1, (data >> 56) & 1, (data >> 61) & 1, (data >> 62) & 1, (data >> 63) & 1);
}

void printMap(int argc, char fileName[]) {
    char* pagemap = (char*)malloc(NAME_SIZE * sizeof(char));
    strcat(pagemap, fileName);
    strcat(pagemap, "/pagemap");
    int fd = open(pagemap, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open pagemap file");
        close(fd);
        return;
    }

    unsigned long long offset = getOffset(fileName);

    for (unsigned long long i = offset; i < 0xffffffffffff; i += PAGE_SIZE) {
        unsigned long long data;
        unsigned long long shift = (i / PAGE_SIZE) * sizeof(data);
        if (pread(fd, &data, sizeof(data), shift) != sizeof(data)) {
            perror("Error reading pagemap");
            close(fd);
            return;
        }
        if (hasInformation(data)) {
            printPageInfo(i, data);
        }
        else {
            printf("virtual address: 0x%-12llx, data:0x%-12llx\n", i, data);
        }
        usleep(400000);
    }
    close(fd);
    free(pagemap);
}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        printf("program does not have arguments\n");
        return 0;
    }
    char filename[NAME_SIZE] = "/proc/self";
    printMap(argc, argv[1]);
    return 0;
}