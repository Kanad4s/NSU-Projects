#include "../include/outputParser.h"
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netdb.h>

Result printAppInfo(struct sockaddr *addr, socklen_t addrlen) {
    char host[NI_MAXHOST];
    char numericHost[NI_MAXHOST];
    char service[NI_MAXSERV];

    int ret = getnameinfo(addr, addrlen, numericHost, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
    if (ret == -1) {
        goto error;
    }
    ret = getnameinfo(addr, addrlen, host, NI_MAXHOST, NULL, 0, 0);
    if (ret == -1) {
        goto error;
    }
    printf("%s (%s), port %s is alive\n", host, numericHost, service);
    return OK;

error:
    printf("getnameinfo(): %s\n", gai_strerror(ret));
    return ERROR;
}

void disableRcvTimeout(int sockfd) {
    struct timeval time = { 0 };
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time));
}

Result printAppCopies(int sockfd, struct timeval timeout) {
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (ret == -1) {
        printf("setsockopt(): %s", gai_strerror(ret));
        return ERROR;
    }

    printf("ALIVE COPIES:\n");
    for (;;) {
        int msg = 0;
        struct sockaddr src;
        socklen_t srclen;

        Result ret = recieveMessage(sockfd, &msg, &src, &srclen);
        if (ret == TIMED_OUT) {
            break;
        } else if (ret == ERROR) {
            goto error;
        }
        if (msg == msg_alive) {
            ret = printAppInfo(&src, srclen);
            if (ret != OK) {
                goto error;
            }
        }
    }

    disableRcvTimeout(sockfd); 
    return OK;

error:
    disableRcvTimeout(sockfd); 
    return ERROR;
}