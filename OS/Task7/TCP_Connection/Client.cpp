#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Consts.h"

int main() {
	int servSockfd;
	int clientSockfd;
	struct sockaddr_in servSockAddr;
	struct sockaddr_in clientSockAddr;
	memset(&servSockAddr, 0, sizeof(struct sockaddr_in));
	servSockAddr.sin_family = AF_INET;
	servSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servSockAddr.sin_port = htons(PORT);

	clientSockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSockfd == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	if (connect(clientSockfd, (struct sockaddr*)&servSockAddr, sizeof(servSockAddr)) == -1) {
		perror("Connect failed");
		return EXIT_FAILURE;
	}

	while (1) {
		int ret;
		char buff[BUFF_SIZE];
		scanf("%s", buff);
		ret = write(clientSockfd, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			perror("Read failed");
			break;
		}

		ret = read(clientSockfd, buff, sizeof(buff));
		if (ret == -1 || ret == 0) {
			perror("Write failed");
			break;
		}
		printf("%s\n", buff);
	}

	close(clientSockfd);
	return EXIT_SUCCESS;
}
