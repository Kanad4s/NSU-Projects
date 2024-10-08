#include <sys/socket.h>
#include <stdbool.h>
#ifndef NET_DISCOVER
#define NET_DISCOVER

typedef enum Result{
    ERROR,
    OK,
    INTERRUPTED,
    TIMED_OUT
} Result;

extern bool isInterrupted;
extern int msg_alive;
extern int msg_request;

void interaptionSignalHandler(int signo); 
Result setupInterraptionSignalHandler();
Result createMulticastSocket(int *socket, const char *port, const char *ip);
Result sendMessage(int sockfd, int *msg, struct sockaddr *destAddr, socklen_t *addrLen);
Result recieveMessage(int sockfd, int *msg, struct sockaddr *srcAddr, socklen_t *addrlen);

#endif