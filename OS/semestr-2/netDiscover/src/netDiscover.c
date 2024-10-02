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


enum Result sendMessage(int sockfd, int *msg, struct sockaddr *destAddr, socklen_t *addrLen) {
    ssize_t ret;
    ssize_t sent = 0;
    while (sent < sizeof(msg)) {
        ret = sendto(socket, (char *)msg + sent, sizeof(msg) - sent, 0, destAddr, *addrLen);
        if (ret == -1) {
            printf("sendto(): %s", strerror(errno));
            return ERROR;
        }
        sent += ret;
    }

    return OK;
}

enum Result recieveMessage(int sockfd, int *msg, struct sockaddr *srcAddr, socklen_t *addrlen) {
    ssize_t recieved = 0;
    while (recieved < sizeof(int)) {
        ssize_t ret = recvfrom(sockfd, (char *)msg + recieved, sizeof(int) - recieved, 0, srcAddr, addrlen);
        if (ret == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return TIMED_OUT;
            } else if (errno == EINTR) {
                return INTERRUPTED;
            }
            printf("RecieveMessage(): %s", strerror(errno));
            return ERROR;
        }
        recieved += ret;
    }
    return OK;
}