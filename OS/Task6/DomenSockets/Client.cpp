#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

int main() {
	int servSock;
	int clientSock;
	struct sockaddr_un servSockAddr;
	char* dsock_file = "./dsock";
	memset(&servSockAddr, 0, sizeof(struct sockaddr_un));
	servSockAddr.sun_family = AF_UNIX;
	strcpy(servSockAddr.sun_path, dsock_file);

	clientSock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (clientSock == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	if (connect(clientSock, (struct sockaddr*)&servSockAddr, sizeof(servSockAddr)) == -1) {
		perror("Connect failed");
		return EXIT_FAILURE;
	}

	while (1) {
		int ret;
		char buff[128];
		scanf("%s", buff);
		ret = write(clientSock, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			printf("Read failed");
			close(clientSock);
			return EXIT_FAILURE;
		}
		ret = read(clientSock, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			printf("Write failed");
			close(clientSock);
			return EXIT_FAILURE;
		}
		printf("%s\n", buff);
	}

	return EXIT_SUCCESS;
}