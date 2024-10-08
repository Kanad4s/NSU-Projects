#include "../include/inputParser.h"
#include "../include/consts.h"
#include <stdlib.h>
#include <stdio.h>

void showUsage() {
    printf("Usage:\nnetDiscover <IPv4|IPv6 multicast group ip> [port]\n");
}

void parseInput(int argc, char **argv, const char **port, const char **ip) {
    if (argc == 1) {
        *ip = defaultGroupIP;
        *port = defaultPort;
    } else if (argc == 2) {
        *ip = argv[1];
        *port = defaultPort;
    } else if (argc == 3) {
        *ip = argv[1];
        *port = argv[2];
    } else {
        printf("wrong argument count");
        showUsage();
        exit(EXIT_FAILURE);
    }
}