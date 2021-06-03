#ifndef SOCKET_DEFINES_H
#define SOCKET_DEFINES_H

#include "../defines.h"

#define UDP_DEBUG DEBUG && 1
#define TCP_DEBUG 1
#define SOCKET_DEBUG DEBUG && 1
#define UDP_QUEUE_SIZE 20
#define TCP_QUEUE_SIZE 20
#define MAXLINE 8192 // max buffer to send and receive, based on microsoft standards
// Number of pending connections to queue
#define CONNECTION_QUEUE_SIZE 10


#endif
