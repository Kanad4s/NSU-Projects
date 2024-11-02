#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#define CLIENTS_COUNT 5

int main() {
	int servSockfd;
	int clientSockfd;
	struct sockaddr_un servSockAddr;
	struct sockaddr_un clientSocksAddr;
	char* dsockFile = "./dsock";

	servSockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (servSockfd == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	memset(&servSockAddr, 0, sizeof(struct sockaddr_un));
	servSockAddr.sun_family = AF_UNIX;
	strcpy(servSockAddr.sun_path, dsockFile);
	unlink(dsockFile);

	if (bind(servSockfd, (struct sockaddr*)&servSockAddr, sizeof(servSockAddr)) == -1) {
		perror("Bind failed");
		close(servSockfd);
		return EXIT_FAILURE;
	}

	if (listen(servSockfd, CLIENTS_COUNT) == -1) {
		printf("Listen failed");
		close(servSockfd);
		return EXIT_FAILURE;
	}

	while (1) {
		int len;
		pid_t pid;
		printf("Listening connections on domain socket %s\n", dsockFile);

		memset(&clientSocksAddr, 0, sizeof(struct sockaddr_un));
		clientSockfd = accept(servSockfd, (struct sockaddr*)&clientSocksAddr, &len);
		if (clientSockfd == -1) {
			perror("Accept failed");
			close(servSockfd);
			return EXIT_FAILURE;
		}
		len = sizeof(clientSocksAddr);
		if (getpeername(clientSockfd, (struct sockaddr*)&clientSocksAddr, &len) == -1) {
			perror("getpeername() failed");
			close(servSockfd);
			close(clientSockfd);
			return EXIT_FAILURE;
		}

		pid = fork();
		if (pid == -1) {
			perror("Pid failed");
			close(servSockfd);
			close(clientSockfd);
			return EXIT_FAILURE;
		}

		if (pid == 0)
			break;

		close(clientSockfd);
	}


	while (1) {
		int ret;
		char buff[128];

		for (int i = 0; i < 128; ++i)
			buff[i] = '\0';

		ret = read(clientSockfd, buff, sizeof(buff));
		if (ret <= -1 || ret == 0) {
			perror("Read failed");
			close(clientSockfd);
			return EXIT_FAILURE;
		}

		printf("Recieved message: %s\n", buff);

		ret = write(clientSockfd, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			printf("Write failed");
			close(clientSockfd);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}