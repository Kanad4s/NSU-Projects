#ifndef OUTPUT_PARSER
#define OUTPUT_PARSER
#include "netDiscover.h"

Result printAppInfo(struct sockaddr *addr, socklen_t addrlen);
void disableRcvTimeout(int sockfd);
Result printAppCopies(int sockfd, struct timeval timeout);

#endif