#ifndef BUFFEREDPACKET_H
#define BUFFEREDPACKET_H

#include <stdlib.h>
#include <string.h>

#include "defines.h"

/*********************************
 * buffered_packet.h
 *
 * Custom c++ implementation of the aodv routing protocol. 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

class BufferedPacket{
public:
	BufferedPacket(IP_ADDR destination, int portId, char* buffer, int length, IP_ADDR origIp);
	BufferedPacket();
	BufferedPacket(const BufferedPacket &bufferedPacket);
	~BufferedPacket();

	BufferedPacket& operator=(const BufferedPacket& bufferedPacket);
	void copyPacket(const BufferedPacket &bufferedPacket);

	int getPortId() const;
	char* getBuffer() const;
	int getLength() const;
	IP_ADDR getDestination() const;
	IP_ADDR getOrigin() const;

protected:
	int m_nPortId;
	char* m_pBuffer;
	int m_nLength;
	IP_ADDR m_nDestination;
	IP_ADDR m_nOrigin;
};

#endif
