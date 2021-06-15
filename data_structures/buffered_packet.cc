#include "buffered_packet.h"

BufferedPacket::BufferedPacket(IP_ADDR destination, int portId, char* buffer, int length, IP_ADDR origIP) {
	m_nPortId = portId;
	m_pBuffer = (char *)(malloc(length));
	memcpy(m_pBuffer, buffer, length);
	m_nLength = length;
	m_nDestination = destination;
	m_nOrigin = origIP;
}

BufferedPacket::BufferedPacket(){
	m_nPortId = 0;
	m_nLength = 0;
	m_pBuffer = nullptr;
	m_nDestination = 0;
	m_nOrigin = -1;
}

BufferedPacket::BufferedPacket(const BufferedPacket &bufferedPacket){
	m_nPortId = bufferedPacket.m_nPortId;
	m_nLength = bufferedPacket.m_nLength;
	m_pBuffer = bufferedPacket.getBuffer();
	m_nDestination = bufferedPacket.m_nDestination;
	m_nOrigin = bufferedPacket.m_nOrigin;
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
	m_nOrigin = bufferedPacket.m_nOrigin;
    return *this;
}

void BufferedPacket::copyPacket(const BufferedPacket &bufferedPacket) {
	m_nPortId = bufferedPacket.m_nPortId;
	m_nLength = bufferedPacket.m_nLength;
	m_pBuffer = bufferedPacket.getBuffer();
	m_nDestination = bufferedPacket.m_nDestination;
	m_nOrigin = bufferedPacket.m_nOrigin;
}

int BufferedPacket::getPortId() const{
    return m_nPortId;
}

char* BufferedPacket::getBuffer() const{
    char* bufferCopy = (char *)(malloc(m_nLength));
    memcpy(bufferCopy, m_pBuffer, m_nLength);
    return bufferCopy;
}

int BufferedPacket::getLength() const {
    return m_nLength;
}

IP_ADDR BufferedPacket::getDestination() const {
	return m_nDestination;
}

IP_ADDR BufferedPacket::getOrigin() const {
	return m_nOrigin;
}
