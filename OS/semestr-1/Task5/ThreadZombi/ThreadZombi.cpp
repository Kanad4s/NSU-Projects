﻿#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stddef.h>

int global = 22;

void createProc(int local) {
	int status;
	pid_t pid = fork();
	pid_t pid2;
	if (pid == 0) {
		printf("Child: pid = %d\n", getpid());
		/*printf("Child: parent pid = %d\n", getppid());
		printf("Child: global: %d, address = %p\n", global, &global);
		printf("Child: local: %d, address = %p\n", local, &local);*/
		local = 55555;
		global = 88888;
		/*printf("Child: change vars in child:\n");
		printf("Child: global: %d, address = %p\n", global, &global);
		printf("Child: local: %d, address = %p\n", local, &local);*/
		sleep(5);
		printf("second fork\n");
		pid2 = fork();
		if (pid2 == 0) {
			printf("Grandchild, pid: %d\n", getpid());
			sleep(10);
			exit(10);
		}
		else if (pid2 > 0) {
			printf("Grandchild`s parent, pid: %d\n", getpid());
			sleep(5);
			exit(10);
		}
		//waitpid(getppid(), &status, 0);
		//printf("Child: exit code of parent = %d\n", WEXITSTATUS(status));
		//sleep(10);
		return;
	}
	else if (pid > 0) {
		/*printf("Parent: global: %d, address = %p\n", global, &global);
		printf("Parent: local: %d, address = %p\n", local, &local);*/
		sleep(50);
		//sleep(100);
		//waitpid(pid, &status, 0);
		//wait(&tmp);
		// parent process waits until the end of execution of child process
		//printf("Parent: exit code of child = %d\n", WEXITSTATUS(status));
		sleep(30);
		exit(5);
	}
	else {
		perror("can't create new process\n");
	}
	return;
}

int main() {
	int local = 11;
	/*printf("In function main:\n");
	printf("global: %d, address = %p\n", global, &global);
	printf("local: %d, address = %p\n", local, &local);*/
	printf("Main parent pid = %d\n", getpid());
	printf("Main parent parent pid = %d\n", getppid());
	sleep(5);
	createProc(local);
	return 0;
}