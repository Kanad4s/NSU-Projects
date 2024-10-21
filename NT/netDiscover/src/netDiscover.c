#include "../include/netDiscover.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>

int msg_alive = 1234;
int msg_request = 9876;

static bool isInterrupted = false;

Result multicastAddMembership(int sockfd, int addrFamily, struct sockaddr *bound_addr) {
    char *optval = NULL;
    int level, optname, optlen;
    if (addrFamily == AF_INET) {
        struct sockaddr_in *addr = (struct sockaddr_in *)bound_addr;
        struct ip_mreq mreq;
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        mreq.imr_multiaddr = addr->sin_addr;
        level = IPPROTO_IP;
        optname = IP_ADD_MEMBERSHIP;
        optval = (char *)&mreq;
        optlen = sizeof(mreq);
    } else if (addrFamily == AF_INET6) {
        struct sockaddr_in6 *addr = (struct sockaddr_in6 *) bound_addr;
        struct ipv6_mreq mreq6;
        mreq6.ipv6mr_interface = htonl(INADDR_ANY);
        mreq6.ipv6mr_multiaddr = addr->sin6_addr;
        level = IPPROTO_IPV6;
        optname = IPV6_ADD_MEMBERSHIP;
        optval = (char *)&mreq6;
        optlen = sizeof(mreq6);
    } else {
        printf("Failed to add multicast membership, unknown protocol family\n");
        return ERROR;
    }
    int err;
    err = setsockopt(sockfd, level, optname, optval, optlen);
    if (err == -1) {
        printf("Failed to add multicast membership, setsockopt(): %s\n", strerror(errno));
        return ERROR;
    }

    return OK;
}

Result createMulticastSocket(int* sockfd, const char* port, const char* ip, struct sockaddr *groupAddr,
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

        err = setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); // reuse addr on
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

