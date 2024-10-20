#include "../include/netDiscover.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include <sys/cdefs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int msg_alive = 1234;
int msg_request = 9876;

static bool isInterrupted = false;

Result multicastAddMembership(int sockfd, int addrFamily, struct sockaddr_storage *bound_addr) {
    char *optval = NULL;
    int optlevel, option, optlen;
    if (addrFamily == AF_INET) {
        struct sockaddr_in *addr = (struct sockaddr_in *)bound_addr;
        struct ip_mreq mreq;
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        mreq.imr_multiaddr = addr->sin_addr;
        optlevel = IPPROTO_IP;
        option = IP_ADD_MEMBERSHIP;
        optval = (char *)&mreq;
        optlen = sizeof(mreq);
    } else if (addrFamily == AF_INET6) {
        struct sockaddr_in6 *addr = (struct sockaddr_in6 *) bound_addr;
        struct ipv6_mreq mreq6;
        mreq6.ipv6mr_interface = htonl(INADDR_ANY);
        mreq6.ipv6mr_multiaddr = addr->sin6_addr;
        optlevel = IPPROTO_IPV6;
        option = IPV6_ADD_MEMBERSHIP;
        optval = (char *)&mreq6;
        optlen = sizeof(mreq6);
    } else {
        printf("Failed to add multicast membership, unknown protocol family\n");
        return ERROR;
    }
    int err;
    err = setsockopt(sockfd, optlevel, option, optval, optlen);
    // err = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (err == -1) {
        printf("Failed to add multicast membership, setsockopt(): %s\n", strerror(errno));
        return ERROR;
    }

    return OK;
}

Result createMulticastSocket(int* sockfd, const char* port, const char* ip, struct sockaddr_storage *groupAddr,
                            socklen_t *addrLen) {
    int err;
    struct addrinfo hints;
    struct addrinfo* res;
    const int optval = 1;
    int addrFamily;
    // *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;

    err = getaddrinfo(ip, port, &hints, &res);
    if (err != 0) {
        printf("getaddrinfo(): %s", gai_strerror(err));
        return ERROR;
    }
    
    struct addrinfo* rp;
    for (rp = res; rp != NULL; rp = rp->ai_next) {
        *sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (*sockfd == -1)
            continue;

        err = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)); // reuse addr on
        if (err == -1) {
            printf("setcockopt(), reuse addr: %s", strerror(errno));
            return ERROR;
        }
        if (bind(*sockfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            memcpy(groupAddr, rp->ai_addr, rp->ai_addrlen);
            addrFamily = rp->ai_family;
            *addrLen = rp->ai_addrlen;
            break;
        }
        close(*sockfd);
    }

    freeaddrinfo(res);

    if (rp == NULL) {
        printf("bind(): %s", strerror(errno));
        return ERROR;
    }

    Result ret = multicastAddMembership(*sockfd, addrFamily, groupAddr);
    if (ret != OK)
        goto error;

    return OK;

error:
    close(*sockfd);
    return ERROR;
}

Result sendMessage(int sockfd, int msg, struct sockaddr *destAddr, socklen_t addrLen) {
    ssize_t ret;
    ssize_t sent = 0;
    while (sent < sizeof(msg)) {
        ret = sendto(sockfd, (char*)&msg + sent, sizeof(msg) - sent, 0, destAddr, addrLen);
        if (ret == -1) {
            printf("sendto(): %s\n", strerror(errno));
            return ERROR;
        }
        sent += ret;
    }

    return OK;
}

Result recieveMessage(int sockfd, int *msg, struct sockaddr *srcAddr, socklen_t *addrlen) {
    ssize_t recieved = 0;
    while (recieved < sizeof(int)) {
        ssize_t ret = recvfrom(sockfd, (char*)msg + recieved, sizeof(int) - recieved, 0, srcAddr, addrlen);
        if (ret == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return TIMED_OUT;
            } else if (errno == EINTR) {
                return INTERRUPTED;
            }
            printf("RecieveMessage(): %s\n", hstrerror(errno));
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
    memset(&sigInt, 0, sizeof(sigInt));
    sigInt.sa_handler = interaptionSignalHandler;
    if (sigaction(SIGINT, &sigInt, NULL) != 0) {
        printf("sigaction(): %s\n", hstrerror(errno));
        return ERROR;
    }

    return OK;
}

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
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
                         &timeout, sizeof(timeout));
    if (ret == -1) {
        printf("setsockopt(): %s", gai_strerror(ret));
        return ERROR;
    }

    printf("\t\tALIVE COPIES:\n");
    for (;;) {
        // printf("next:");
        int msg = 0;
        struct sockaddr_storage src;
        socklen_t srclen;

        Result ret = recieveMessage(sockfd, &msg, (struct sockaddr *)&src, &srclen);
        // printf("message got: %d\n", msg);
        if (ret == TIMED_OUT) {
            break;
        } else if (ret == ERROR) {
            goto error;
        }
        if (msg == msg_alive) {
            ret = printAppInfo((struct sockaddr *)&src, srclen);
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