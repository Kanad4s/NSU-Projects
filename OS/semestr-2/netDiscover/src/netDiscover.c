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

#include "../include/inputParser.h"

int create_multicast_socket(int* socket, const char* port, const char* ip) {
    const int optval = 1;
    setsockopt(*socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // addr.sin_addr.s_addr = htonl()
    // struct addrinfo addrinfo1;
    

}

int main(int argc, char* argv[]) {
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    const char* ip;
    const char* port;
    parseInput(argc, argv, port, ip);
    create_multicast_socket(&sfd, port, ip);
}