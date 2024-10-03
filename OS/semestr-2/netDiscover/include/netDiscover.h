#include <sys/socket.h>
#include <stdbool.h>
#ifndef NET_DISCOVER
#define NET_DISCOVER

typedef enum Result {
    ERROR,
    OK,
    INTERRUPTED,
    TIMED_OUT
};

extern bool isInterrupted;

enum Result createMulticastSocket(int *socket, const char *port, const char *ip);
enum Result sendMessage(int sockfd, int *msg, struct sockaddr *destAddr, socklen_t *addrLen);
enum Result recieveMessage(int sockfd, int *msg, struct sockaddr *srcAddr, socklen_t *addrlen);
enum Result setupInterraptionSignalHandler();

#endif