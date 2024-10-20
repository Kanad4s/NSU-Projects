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
#include "../include/netDiscover.h"

int main(int argc, char **argv) {
    const char* ip;
    const char* port;
    struct sockaddr_storage groupAddr;
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
    struct sockaddr groupaddr;
    socklen_t groupaddrLen;

    ret = sendMessage(sockfd, &msg_request, &groupaddr, &groupaddrLen);
    if (ret != OK) {
        goto error;
    }

    while (!isInterrupted) {
        int msg;
        ret = recieveMessage(sockfd, &msg, &groupaddr, &groupaddrLen);
        if (ret == INTERRUPTED) {
            break;
        } else if (ret != OK) {
            goto error;
        }
        if (msg == msg_request) {
            ret = sendMessage(sockfd, &msg_alive, &groupaddr, &groupaddrLen);
            if (ret != OK) {
                goto error;
            }
            ret = printAppCopies(sockfd);
            if (ret != OK) {
                goto error;
            }
        }
    }

    printf("Finish\n");

    ret = sendMessage(sockfd, &msg_request, &groupaddr, &groupaddrLen);
    if (ret != OK) {
        goto error;
    }
    close(sockfd);
    return EXIT_SUCCESS;

error:
    printf("\nerror from main\n");
    close(sockfd);
    return EXIT_FAILURE;
}