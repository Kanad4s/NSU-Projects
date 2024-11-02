#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <poll.h>
#include "Consts.h"

void closeSock(int* socks, int s) {
	for (int i = 0; i < CLIENTS_COUNT; ++i) {
		if (socks[i] == s) {
			close(socks[i]);
			socks[i] = -1;
			return;
		}
	}
}

void closeAllSocks(int* socks) {
	for (int i = 0; i < CLIENTS_COUNT; ++i) {
		close(socks[i]);
	}
}

int main() {
	int servSockfd;
	int clientSocks[CLIENTS_COUNT];
	struct sockaddr_in servSockAddr;
	struct sockaddr_in clientSockAddr;
	int len, err, s;
	char buff[1024];

	signal(SIGSEGV, SIG_IGN);
	memset(clientSocks, -1, sizeof(clientSocks));
	servSockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (servSockfd == -1) {
		perror("Socket failed");
		return EXIT_FAILURE;
	}

	memset(&servSockAddr, 0, sizeof(struct sockaddr_in));
	servSockAddr.sin_family = AF_INET;
	servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servSockAddr.sin_port = htons(PORT);

	if (bind(servSockfd, (struct sockaddr*)&servSockAddr, sizeof(servSockAddr)) == -1) {
		perror("Bind failed");
		close(servSockfd);
		return EXIT_FAILURE;
	}

	if (listen(servSockfd, 5) == -1) {
		perror("Listen failed");
		close(servSockfd);
		return EXIT_FAILURE;
	}

	while (1) {
		struct pollfd fds[CLIENTS_COUNT];
		int ret;
		int fdsCount = 0;
		int timeout = 5000;

		fds[0].fd = servSockfd;
		fds[0].events = POLLIN;
		fds[0].revents = 0;
		fdsCount++;

		for (int i = 0; i < CLIENTS_COUNT; ++i) {
			if (clientSocks[i] != -1) {
				printf("Add fd: %d, index in array %d\n", clientSocks[i], i);
				fds[fdsCount].fd = clientSocks[i];
				fds[fdsCount].events = POLLIN;
				fds[fdsCount].revents = 0;
				fdsCount++;
			}
		}

		// if something happened on file descriptors then...
		ret = poll(fds, fdsCount, timeout);
		printf("Poll returned %d\n", ret);
		if (ret == 0) {
			printf("no activity on the sockets for >= 5 seconds\n");
			continue;
		}
		else if (ret > 0) {
			if (fds[0].revents == POLLIN) {
				printf("Ready to accept new connection\n");
				memset(&clientSockAddr, 0, sizeof(struct sockaddr_in));
				clientSocks[s] = accept(servSockfd, (struct sockaddr*)&clientSockAddr, &len);
				if (clientSocks[s] == -1) {
					perror("Accept failed, continue execution on the next iteration");
					continue;
					//close(servSockfd);
					//return EXIT_FAILURE;
				}
				s++;
			}

			for (int i = 1; i < fdsCount; ++i) {
				if (fds[i].revents == POLLIN) {
					printf("POLLIN event on socket %d\n", fds[i].fd);
					ret = read(fds[i].fd, buff, sizeof(buff));
					printf("index: %d, ret: %d\n", i, ret);
					if (ret == -1 || ret == 0) {
						perror("Read failed");
						closeSock(clientSocks, fds[i].fd);
						fdsCount--;
						break;
					}
					ret = write(fds[i].fd, buff, ret);
					if (ret == -1) {
						perror("Write failed");
						closeSock(clientSocks, fds[i].fd);
						fdsCount--;
						break;
					}
				}
			}
		}
	}

	closeAllSocks(clientSocks);

	return EXIT_SUCCESS;
}