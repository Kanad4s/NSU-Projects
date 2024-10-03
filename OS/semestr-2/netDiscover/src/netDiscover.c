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
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#define MSG_ALIVE 0x1234
#define MSG_DEAD 0xABCD

bool isInterrupted = false;

Result createMulticastSocket(int* sockfd, const char* port, const char* ip) {
    int err;
    const int optval = 1;
    struct in_addr ipToNum;
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
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
    err = bind(*sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (err != 0) {
        printf("bind(): %s", hstrerror(err));
        goto error;
    }

    return OK;

error:
    close(*sockfd);
    return ERROR;
}


Result sendMessage(int sockfd, int *msg, struct sockaddr *destAddr, socklen_t *addrLen) {
    ssize_t ret;
    ssize_t sent = 0;
    while (sent < sizeof(msg)) {
        ret = sendto(sockfd, (char *)msg + sent, sizeof(msg) - sent, 0, destAddr, *addrLen);
        if (ret == -1) {
            printf("sendto(): %s", hstrerror(errno));
            return ERROR;
        }
        sent += ret;
    }

    return OK;
}

Result recieveMessage(int sockfd, int *msg, struct sockaddr *srcAddr, socklen_t *addrlen) {
    ssize_t recieved = 0;
    while (recieved < sizeof(int)) {
        ssize_t ret = recvfrom(sockfd, (char *)msg + recieved, sizeof(int) - recieved, 0, srcAddr, addrlen);
        if (ret == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return TIMED_OUT;
            } else if (errno == EINTR) {
                return INTERRUPTED;
            }
            printf("RecieveMessage(): %s", hstrerror(errno));
            return ERROR;
        }
        recieved += ret;
    }
    return OK;
}

void interaptionSignalHandler(int signo) {
    isInterrupted = true;
}

Result setupInterraptionSignalHandler() {
    struct sigaction sigInt;
    bzero(&sigInt, sizeof(sigInt));
    sigInt.sa_handler = interaptionSignalHandler;
    if (sigaction(SIGINT, &sigInt, NULL) != 0) {
        printf("sigaction(): %s", hstrerror(errno));
        return ERROR;
    }

    return OK;
}