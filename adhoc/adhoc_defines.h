#ifndef ADHOC_DEFINES
#define ADHOC_DEFINES

#include "defines.h"


#define MONITOR_DEBUG       DEBUG && 1
#define ROUTING_DEBUG       DEBUG && 1
#define TABLE_DEBUG         DEBUG && 1

#define BYTE_SIZE				1
#define ADDRESS_SIZE			(4 * BYTE_SIZE)

/*
 * Packet header defines
 * Header format: [HEADER_LEAD_BYTE, HEADER_DESTINATION, HEADER_ORIGIN]
 *
 * NOTE: These defines (although nice looking and fun to use) have not
 * yet been universally adopted throughout the project. If you want to
 * change the format of the header you will have to update project. See
 * issue #43.
 */
#define HEADER_LEAD_BYTE		0
#define HEADER_DESTINATION		(HEADER_LEAD_BYTE + BYTE_SIZE)
#define HEADER_ORIGIN			(HEADER_DESTINATION + ADDRESS_SIZE)
#define HEADER_SIZE				(HEADER_ORIGIN + ADDRESS_SIZE)

#define PACKET_BUFFER_SIZE		0

#endif // ADHOC_DEFINES
