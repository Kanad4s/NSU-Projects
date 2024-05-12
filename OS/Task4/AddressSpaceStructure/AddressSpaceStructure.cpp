#include <iostream>
//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
﻿#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int globalNotInit;
int globalInit = 10;
const int globalConst = 5;


void function() {
    int localFunc = 10;
    int localFuncNot;
    static int staticFunc;
    printf("func, usual func: %p\n", (void*)&localFunc);
    printf("func, not init: %p\n", (void*)&localFuncNot);
    printf("func, static func: %p\n", (void*)&staticFunc);
}

void* returnAddress() {
    int var = 5;
    printf("func, var in func %p\n", &var);
    return (void*)&var;
}

void printBuf(char buffer[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

void createBuffer() {
    int bufSize = 100;
    char* buffer = (char*)malloc(bufSize * sizeof(char));
    strcat(buffer, "hello world!");
    printf("buffer1 add: ");
    printBuf(buffer, bufSize);
    free(buffer);
    printf("buffer1 free: ");
    printBuf(buffer, bufSize);
    //printf("%s\n", buffer);
    char* buffer2 = (char*)malloc(bufSize * sizeof(char));
    strcat(buffer2, "Bye world!");
    printf("buffer2 add: ");
    printBuf(buffer2, bufSize);
    //buffer2 += bufSize / 2;
    free(buffer2);
    printf("buffer2 free: ");
    printBuf(buffer2, bufSize);
}

void createBuffer2()
{
    int bufSize = 100;
    char* buffer = (char*)malloc(bufSize * sizeof(char));
    for (int i = 0; i < 10; ++i)
        buffer[i] = '0' + i;
    printf("buffer1 add: ");
    printBuf(buffer, bufSize);
    free(buffer);
    printf("buffer1 free: ");
    printBuf(buffer, bufSize);
    char* buffer2 = (char*)malloc(bufSize * sizeof(char));
    for (int i = 0; i < 10; ++i)
        buffer[i] = '9' - i;
    printf("buffer2 add: ");
    printBuf(buffer2, bufSize);
    //buffer2 += 50;
    free(buffer2);
    printf("buffer2 free: ");
    printBuf(buffer2, bufSize);
}


int main()
{
    int localMain = 1;
    static int staticMain = 2;
    const int constVar = 5;
    printf("pid: %d", getpid());
    printf("Local, usual: %p\n", (void*)&localMain);
    printf("Local, static: %p\n", (void*)&staticMain);
    printf("Local, const: %p\n", (void*)&constVar);
    function();
    printf("global init: %p\n", (void*)&globalInit);
    printf("global uninit: %p\n", (void*)&globalNotInit);
    printf("global const: %p\n", &globalConst);
    //printf("func, var out func: %p\n", returnAddress());
    createBuffer2();
    sleep(20);
}