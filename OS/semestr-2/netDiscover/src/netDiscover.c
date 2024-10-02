#include "../include/netDiscover.h"

#include <features.h>
#include <bits/stdint-uintn.h>
#include <bits/types.h>

#include <sys/cdefs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "netDiscover.h"
#include <errno.h>

enum Result createMulticastSocket(int* socket, const char* port, const char* ip) {
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
}


enum Result sendMessage(int sockfd, char *msg, struct sockaddr *destAddr, socklen_t destAddrLen) {
    ssize_t ret;
    ssize_t sent = 0;
    while (sent < sizeof(msg)) {
        ret = sendto(socket, msg + sent, sizeof(msg) - sent, 0, destAddr, destAddrLen);
        if (ret == -1) {
            printf("sendto(): %s", strerror(errno));
            return ERROR;
        }
        sent += ret;
    }

    return OK;
}