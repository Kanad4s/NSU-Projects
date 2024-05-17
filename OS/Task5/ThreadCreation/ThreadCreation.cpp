#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global = 22;

void printVars(int globalVar, int localVar) {
	printf("global: %d, address = %p\n", globalVar, &globalVar);
	printf("local: %d, address = %p\n", localVar, &localVar);
}

void createProc(int local) {
	int tmp;
	pid_t pid = fork();
	if (pid == 0) {
		printf("Child: pid = %d\n", getpid());
		printf("Child: parent pid = %d\n", getppid());
		printf("Child: global: %d, address = %p\n", global, &global);
		printf("Child: local: %d, address = %p\n", local, &local);
		local = 55555;
		global = 88888;
		printf("Child: change vars in child:\n");
		printf("Child: global: %d, address = %p\n", global, &global);
		printf("Child: local: %d, address = %p\n", local, &local);
		exit(5);
	}
	else if (pid > 0) {
		printf("Parent: global: %d, address = %p\n", global, &global);
		printf("Parent: local: %d, address = %p\n", local, &local);
		sleep(5);
		wait(&tmp);
		// parent process waits until the end of execution of child process
		printf("Parent: exit code of child = %d\n", WEXITSTATUS(tmp));
	}
	else {
		perror("can't create new process\n");
	}
	return;
}

int main() {
	int local = 11;
	printf("In function main:\n");
	printf("global: %d, address = %p\n", global, &global);
	printf("local: %d, address = %p\n", local, &local);
	printf("Main parent pid = %d\n", getpid());
	printf("Main parent parent pid = %d\n", getppid());
	sleep(10);
	createProc(local);
	return 0;
}
