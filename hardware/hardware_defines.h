#ifndef UDP_DEFINES_H
#define UDP_DEFINES_H

#include "defines.h"

#define HARDWARE_DEBUG DEBUG && 1
#define RSSI_DEBUG HARDWARE_DEBUG && 1

#define RSSI_NETWORK_FILER ""

/*!
 * @brief prints the data of a packet to the specified filed
 *
 * @param file  where to print the packet
 * @param buffer the packet to print
 * @param length the length of the packet
 */
void printPacket(FILE* file, char * buffer, int length);

#endif
