#include "buffered_packet.h"

BufferedPacket::BufferedPacket(IP_ADDR destination, int portId, char* buffer, int length) {
	m_nPortId = portId;
	m_pBuffer = (char *)(malloc(length));
	memcpy(m_pBuffer, buffer, length);
	m_nLength = length;
	m_nDestination = destination;
}

BufferedPacket::BufferedPacket(){
	m_nPortId = 0;
	m_nLength = 0;
	m_pBuffer = nullptr;
	m_nDestination = 0;
}

BufferedPacket::BufferedPacket(const BufferedPacket &bufferedPacket){
	m_nPortId = bufferedPacket.m_nPortId;
	m_nLength = bufferedPacket.m_nLength;
	m_pBuffer = bufferedPacket.getBuffer();
	m_nDestination = bufferedPacket.m_nDestination;
}

BufferedPacket::~BufferedPacket(){
    free(m_pBuffer);
}

BufferedPacket& BufferedPacket::operator=(const BufferedPacket& bufferedPacket){
    m_nPortId = bufferedPacket.m_nPortId;
    free(m_pBuffer);
    m_pBuffer = bufferedPacket.getBuffer();
    m_nLength = bufferedPacket.m_nLength;
	m_nDestination = bufferedPacket.m_nDestination;
    return *this;
}

void BufferedPacket::copyPacket(const BufferedPacket &bufferedPacket) {
	m_nPortId = bufferedPacket.m_nPortId;
	m_nLength = bufferedPacket.m_nLength;
	m_pBuffer = bufferedPacket.getBuffer();
	m_nDestination = bufferedPacket.m_nDestination;
}

int BufferedPacket::getPortId() {
    return m_nPortId;
}

char* BufferedPacket::getBuffer() const{
    char* bufferCopy = (char *)(malloc(m_nLength));
    memcpy(bufferCopy, m_pBuffer, m_nLength);
    return bufferCopy;
}

int BufferedPacket::getLength() {
    return m_nLength;
}

IP_ADDR BufferedPacket::getDestination() {
	return m_nDestination;
}
