#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mythread.h"

void* mythread(void* arg) {
    char* str = (char*)arg;

    for(int i = 0; i<4; i++) {
        printf("hello from thread '%s'\n", str);
        sleep(1);
    }
    return "bay";
}

int main() {
    mythread_t mytid;
    void* retval;

    printf("main [%d %d]: Hello from main!\n", getpid(), getppid());

    int err = mythread_create(&mytid, mythread, "hello from main");

    if (err == 0) {
        printf("main [%d %d] thread returned '%s'\n", getpid(), getppid(), (char*)retval);
    } else {
        printf("Failed to create thread\n");
    }

    mythread_join(mytid, &retval);
    
    return 0;
}
