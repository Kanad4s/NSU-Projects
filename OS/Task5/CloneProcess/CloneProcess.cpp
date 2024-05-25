#define _GNU_SOURCE
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sched.h>
#include <linux/sched.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <string.h>

#define SEG_SIZE 0x1000
#define DEPTH 10

long long myStackPointer;
int fd;

void* createStack() {
	fd = open("memory", O_RDWR | O_CREAT, 0660);
	if (fd == -1) {
		perror("open");
		exit(-1);
	}

	ftruncate(fd, 0x0);
	ftruncate(fd, SEG_SIZE);
	void* stack = mmap(NULL, SEG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (stack == MAP_FAILED) {
		perror("Stack failed");
		exit(EXIT_FAILURE);
	}

	myStackPointer = (long long)stack;
	return stack;
}

void recursiveHello(int depth) {
	const char helloWorld[] = "Hello World!";
	if (depth <= 0)
		return;

	printf("Current stack position: 0x%-16llx\n", myStackPointer);

	char* stackChar = (char*)myStackPointer;

	/*for (int i = 0; i < 13; ++i)
		stackChar[i] = helloWorld[i];*/

	myStackPointer += strlen(helloWorld);

	void* returnAddress = __builtin_extract_return_addr(__builtin_return_address(0));
	printf("Return address of recursiveHello(%d): %p\n", depth, returnAddress);
	printf("Depth address: %p\n", &depth);
	recursiveHello(depth - 1);

}

int entryPointChild(void* stack) {
	recursiveHello(DEPTH);
	//exit(EXIT_SUCCESS);
	return 0;
}

int main() {
	void* stack = createStack();
	void* stackTop = stack + SEG_SIZE;

	printf("Stack: %p, Stack top: %p\n", stack, stackTop);

	pid_t pid = clone(entryPointChild, stackTop, SIGCHLD, NULL);
	if (pid == -1) {
		perror("Clone failed");
		return EXIT_FAILURE;
	}
	printf("Pid of cloned process: %d\n", pid);
	printf("Current pid: %d\n", getpid());
	sleep(100);
	return EXIT_SUCCESS;
}