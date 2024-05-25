#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include "Consts.h"

int main() {
	int servSockfd, clientSockfd, err;
	struct sockaddr_in srvSockAddr;
	struct sockaddr_in clientSockAddr;

	servSockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (servSockfd == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	memset(&srvSockAddr, 0, sizeof(struct sockaddr_in));
	srvSockAddr.sin_family = AF_INET;
	srvSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvSockAddr.sin_port = htons(PORT);

	if (bind(servSockfd, (struct sockaddr*)&srvSockAddr, sizeof(srvSockAddr)) == -1) {
		perror("Bind failed");
		close(servSockfd);
		return EXIT_FAILURE;
	}

	while (1) {
		char buff[128];
		for (int i = 0; i < 128; ++i) {
			buff[i] = '\0';
		}
		int len = sizeof(clientSockAddr);
		err = recvfrom(servSockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientSockAddr, &len);
		if (err == -1 || err == 0) {
			perror("Read failed");
			break;
		}
		printf("Message reveived: %s\n", buff);
		err = sendto(servSockfd, buff, sizeof(buff), 0, (struct sockaddr*)&clientSockAddr, sizeof(clientSockAddr));
		if (err == -1 || err == 0) {
			perror("Write failed");
			break;
		}
	}

	close(servSockfd);

	return EXIT_SUCCESS;
}
