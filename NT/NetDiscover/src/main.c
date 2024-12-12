#include "../include/inputParser.h"
#include "../include/outputParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    const char* ip;
    const char* port;
    struct sockaddr groupAddr;
    socklen_t groupAddrLen;
    int sockfd;
    parseInput(argc, argv, &port, &ip);
    Result ret = setupInterraptionSignalHandler();
    if (ret != OK) {
        goto error;
    }
    
    ret = createMulticastSocket(&sockfd, port, ip, &groupAddr, &groupAddrLen);
    if (ret != OK) {
        goto error;
    }

    ret = sendMessage(sockfd, msg_request, &groupAddr, groupAddrLen);
    if (ret != OK) {
        goto error;
    }

    while (!isInterrupted) {
        int msg;
        struct sockaddr src;
        socklen_t srcLen;
        ret = recieveMessage(sockfd, &msg, &src, &srcLen);
        if (ret == INTERRUPTED) {
            break;
        } else if (ret != OK) {
            goto error;
        }
        if (msg == msg_request) {
            ret = sendMessage(sockfd, msg_alive, &groupAddr, groupAddrLen);
            if (ret != OK) {
                goto error;
            }
            struct timeval collectTimeout;
            collectTimeout.tv_sec = 0;
            collectTimeout.tv_usec = 500000;
            ret = printAppCopies(sockfd, collectTimeout);
            if (ret != OK) {
                goto error;
            }
        }
    }

    printf("Finish\n");

    ret = sendMessage(sockfd, msg_request, &groupAddr, groupAddrLen);
    if (ret != OK) {
        goto error;
    }
    close(sockfd);
    return EXIT_SUCCESS;

error:
    close(sockfd);
    return EXIT_FAILURE;
}