#ifndef AODV_DEFINES_H
#define AODV_DEFINES_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "defines.h"
#include "adhoc_defines.h"

#define GLOBAL_AODV_DEBUG   0

#define AODV_PRINT_PACKET   1 && GLOBAL_AODV_DEBUG
#define AODV_LOG_OUTPUT	    1 && GLOBAL_AODV_DEBUG
#define AODV_DEBUG		    DEBUG && 1 && GLOBAL_AODV_DEBUG
#define RREQ_DEBUG		    DEBUG && 1 && GLOBAL_AODV_DEBUG
#define RREP_DEBUG		    DEBUG && 1 && GLOBAL_AODV_DEBUG
#define RERR_DEBUG          DEBUG && 1 && GLOBAL_AODV_DEBUG
#define AODV_WARNING        0

#endif
