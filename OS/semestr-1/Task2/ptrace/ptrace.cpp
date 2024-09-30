#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

void child() {
	ptrace(PTRACE_TRACEME, 0, 0, 0);
	execl("/bin/echo", "/bin/echo", "Hello, world!", NULL);
	perror("execl");
}

void parent(pid_t pid) {
	int status;
	waitpid(pid, &status, 0);
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
	while (!WIFEXITED(status)) {
		struct user_regs_struct state;
		ptrace(PTRACE_SYSCALL, pid, 0, 0);
		waitpid(pid, &status, 0);
		// at syscall
		//при остановке на системном вызове родительский процесс получит в статусе SIGTRAP | 0x80
		//breakpoint -  PTRACE_SINGLESTEP или заменой интсрукции на 0xCC
		//отмена вызова функции PTRACE_SYSEMU
		if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
			ptrace(PTRACE_GETREGS, pid, 0, &state);
			printf("SYSCALL %lld at %08llx\n", state.orig_rax, state.rip);

			// sys_write
			if (state.orig_rax == 1) {
				char* text = (char*)state.rsi;
				ptrace(PTRACE_POKETEXT, pid, (void*)(text + 7), 0x61686168); //72626168 - habr 6f6a6c6a7562 - nikita
				ptrace(PTRACE_POKETEXT, pid, (void*)(text + 11), 0x00000a21); //!\n
			}
			// skip after syscall
			ptrace(PTRACE_SYSCALL, pid, 0, 0);
			waitpid(pid, &status, 0);
		}
	}
}

int main(int argc, char** argv) {
	pid_t pid = fork();
	if (pid == 0)
		child();
	else if (pid == -1) {
		printf("can't create process\n");
		return EXIT_FAILURE;
	}
	else {
		parent(pid);
	}
	return EXIT_SUCCESS;
}