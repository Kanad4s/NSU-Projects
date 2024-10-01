#ifndef NET_DISCOVER
#define NET_DISCOVER

typedef enum Result {
    ERROR,
    OK
};

enum Result createMulticastSocket(int* socket, const char* port, const char* ip);

#endif