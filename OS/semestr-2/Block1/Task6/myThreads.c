#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sched.h>
#include <errno.h>
#include <signal.h>
#include <ucontext.h>

#define PAGE 4096
#define STACK_SIZE PAGE * 8
#define THREADS_NUM 10

typedef void *(*startRoutine_t)(void*);

typedef struct _myThread {
		int myThreadId;
		startRoutine_t startRoutine;
		void* arg;
		void* retVal;
		volatile int joined;
		volatile int exited;

		volatile int canceled;
		ucontext_t beforeStartRoutine;
} myThread_struct_t;

typedef myThread_struct_t* myThread_t;
myThread_t structs[THREADS_NUM];
int tids[THREADS_NUM];

int myThreadStartup(void* arg) { 
	myThread_struct_t* myThread = (myThread_struct_t*) arg;
	getcontext(&(myThread->beforeStartRoutine));
	printf("myThreadStartup: canceled %d\n", myThread->canceled);
	if (!myThread->canceled) {
		myThread->retVal = myThread->startRoutine(myThread->arg);
	}
	myThread->exited = 1;
	
	while(!myThread->joined) {
		sleep(1);
	}
	return 0;
}

void* createStack(off_t size, int threadNum) {
	int stackFd;
	void* stack;

	char* stackFile = malloc(sizeof("stack-***"));
	if (stackFile == NULL) return NULL;

	int result = snprintf(stackFile, sizeof(stackFile), "stack-%d", threadNum);
	if (result == 0) {
		free(stackFile);
		return NULL;
	}

	stackFd = open(stackFile, O_RDWR | O_CREAT, 0660);
	if (stackFd == -1) {
		free(stackFile);
		return NULL;
	}
	free(stackFile);

	ftruncate(stackFd, 0);
	ftruncate(stackFd, size);

	stack = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, stackFd, 0);
	if (stack == NULL) {
		close(stackFd);
		return NULL;
	}

	close(stackFd);
	memset(stack, 0x7f, size);
	return stack;
}

int myThreadCreate(myThread_t* tid, startRoutine_t startRoutine, void* arg) {
	static int threadNum = 0;
	threadNum++;

	void* childStack = createStack(STACK_SIZE + sizeof(myThread_struct_t), threadNum);
	if (childStack == NULL) return -1;
	printf("stack for thread %d : %p %p\n", threadNum, childStack, childStack + STACK_SIZE);
	myThread_struct_t* myThread = (myThread_struct_t*) (childStack + STACK_SIZE);
	myThread->myThreadId = threadNum;
	myThread->startRoutine = startRoutine;
	myThread->arg = arg;
	myThread->exited = 0;
	myThread->joined = 0;
	myThread->canceled = 0;

	int childPid = clone(myThreadStartup, childStack + STACK_SIZE, CLONE_VM | CLONE_FILES |
                     CLONE_THREAD | CLONE_SIGHAND | SIGCHLD, (void*)myThread);
	if (childPid == -1) {
		printf("clone failed: %s\n", strerror(errno));	
		munmap(childStack, STACK_SIZE + sizeof(myThread_struct_t));
		return -1;
	}
	tids[threadNum] = childPid;
	*tid = myThread;
	structs[threadNum] = myThread;
	return 0;
}

myThread_t myThreadSelf(void) {
        int id = gettid();
        int tid = 0;
        for (int i = 1; i < THREADS_NUM; ++i) {
                if (tids[i] == id) tid = i;
        }
        return structs[tid];
}

int myThreadJoin(myThread_t myTid, void** retVal) {
	myThread_struct_t* myThread = myTid;
	
	while (!myThread->exited) {
		sleep(1);
	}
	*retVal = myThread->retVal;
	myThread->joined = 1;

	return 0;
}

void myThreadCancel(myThread_t tid) {
	myThread_struct_t* thread = tid;

	printf("thread cancel: set cancel for thread %d\n", thread->myThreadId);
	thread->retVal = "canceled";
	thread->canceled = 1;
}

void myThreadTestCancel(void) {
	myThread_struct_t* thread = myThreadSelf();
	printf("thread_testcancel: check cancel %d\n", thread->canceled);
	if (thread->canceled) {
		setcontext(&(thread->beforeStartRoutine));
	}
}
 
void* myThread(void* arg) {
	printf("myThread function is executing\n");
	char* str = (char*) arg;

	for (size_t i = 0; i < 5; ++i) {
		char string[128];
		snprintf(string, sizeof(string), "%s\n", str);
		write(1, string, sizeof(string));
		myThreadTestCancel();
		sleep(1);
	}
	
	printf("tid: %d\n", gettid());
	return "bye";
}

int main() {
	printf("pid %d\n", getpid());
	myThread_t tid1;
	myThread_t tid2;

	int err = myThreadCreate(&tid1, myThread, "hello from thread1");
	if (err != 0) {
		printf("myThreadCreate failed for thread1\n");
		return -1;
	}
	err = myThreadCreate(&tid2, myThread, "hello from thread2");
        if (err != 0) {
                printf("myThreadCreate failed for thread2\n");
                return -1;
        }

	sleep(2);
	myThreadCancel(tid1);

	void* retVal;
	err = myThreadJoin(tid1, (void**) &retVal);
	if (err != 0) {
		printf("myThreadJoin failed for thread1\n");
		return -1;
	}
	printf("retVal of thread1 in main: %s\n", (char*)retVal);
	err = myThreadJoin(tid2, (void**) &retVal);
        if (err != 0) {
                printf("myThreadJoin failed for thread2\n");
                return -1;
        }
        printf("retVal of thread2 in main: %s\n", (char*)retVal);
	
	return 0;
}