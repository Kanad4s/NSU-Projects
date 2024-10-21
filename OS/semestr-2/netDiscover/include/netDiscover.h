#include <sys/socket.h>
#include <stdbool.h>
#include <time.h>
#ifndef NET_DISCOVER
#define NET_DISCOVER

typedef enum Result{
    ERROR,
    OK,
    INTERRUPTED,
    TIMED_OUT
} Result;

static bool isInterrupted;
extern int msg_alive;
extern int msg_request;

void interaptionSignalHandler(int signo); 
Result setupInterraptionSignalHandler();
Result multicastAddMembership(int sockfd, int addrFamily, struct sockaddr *bound_addr);
Result createMulticastSocket(int* sockfd, const char* port, const char* ip, struct sockaddr *groupAddr, socklen_t *addrLen);
Result sendMessage(int sockfd, int msg, struct sockaddr *destAddr, socklen_t addrLen);
Result recieveMessage(int sockfd, int *msg, struct sockaddr *srcAddr, socklen_t *addrlen);
Result printAppCopies(int sockfd, struct timeval timeout);
Result printAppInfo(struct sockaddr *addr, socklen_t addrlen);

#endif