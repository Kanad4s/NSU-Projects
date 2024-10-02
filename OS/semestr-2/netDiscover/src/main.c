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

int main(int argc, char* argv[]) {
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    const char* ip;
    const char* port;
    parseInput(argc, argv, port, ip);
    enum Result ret = createMulticastSocket(&sfd, port, ip);
    if (ret != OK) {
        goto error;
    }

error:
    close(sfd);
    return EXIT_FAILURE;
}