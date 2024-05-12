#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

size_t GB = 1024 * 1024 * 1024;
size_t ARR_SIZE = 512 * 1024 * 1024;

int iteration = 0;
int start_deletion = 0;

void killHeap() {
	char* a = (char*)malloc(ARR_SIZE * sizeof(char));
	++iteration;
	printf("%d, allocated memory %f GB \n", iteration, ARR_SIZE * iteration * sizeof(char) / GB);
	vsleep(100000);
	if (a != NULL || start_deletion == 1) {
		//for (size_t i = 0; i < ARR_SIZE; ++i)
			//a[i] = 'a';
		killHeap();
	}
	else {
		free(a);
		printf("free, iteration %d\n", iteration--);
	}
}

void handler(int sig)
{
	printf("SIGSEGV found");
	signal(sig, SIG_IGN);
	start_deletion = 1;
	exit(-1);
}

int main()
{
	signal(SIGSEGV, &handler);

	printf("Pid: %d\n", getpid());
	int* a = (int*)malloc(ARR_SIZE * sizeof(int));
	sleep(10);
	killHeap();
	return 0;
}