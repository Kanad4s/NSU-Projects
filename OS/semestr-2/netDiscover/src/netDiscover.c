#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include <features.h>
#include <bits/stdint-uintn.h>
#include <bits/types.h>

#include <sys/cdefs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../include/inputParser.h"

typedef enum Result {
    ERROR,
    OK
};

enum Result create_multicast_socket(int* socket, const char* port, const char* ip) {
    int err;
    const int optval = 1;
    setsockopt(*socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    struct in_addr ipToNum;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    err = inet_pton(AF_INET, ip, &ipToNum);
    if (err <= 0) {
        printf("Error in IP translation to special numeric format\n");
        goto error;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr = ipToNum;
    err = bind(socket, (struct sockaddr *)&addr, sizeof(addr));
    if (err != 0) {
        printf("bind(): %s", strerror(err));
        goto error;
    }

    return OK;

error:
    close(socket);
    return ERROR;


    struct addrinfo* hints;
    struct addrinfo* result;
    err = getaddrinfo(ip, port, hints, &result);
    if (err != 0) {
        printf("getaddrinfo(): %s", gai_strerror(err));
        return ERROR;
    }
}

int main(int argc, char* argv[]) {
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    const char* ip;
    const char* port;
    parseInput(argc, argv, port, ip);
    create_multicast_socket(&sfd, port, ip);
}