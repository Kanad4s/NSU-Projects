#include <sys/socket.h>
#include <stdbool.h>
#ifndef NET_DISCOVER
#define NET_DISCOVER

typedef enum{
    ERROR,
    OK,
    INTERRUPTED,
    TIMED_OUT
} Result;

extern bool isInterrupted;

void interaptionSignalHandler(int signo); 
Result setupInterraptionSignalHandler();
Result createMulticastSocket(int *socket, const char *port, const char *ip);
Result sendMessage(int sockfd, int *msg, struct sockaddr *destAddr, socklen_t *addrLen);
Result recieveMessage(int sockfd, int *msg, struct sockaddr *srcAddr, socklen_t *addrlen);

#endif