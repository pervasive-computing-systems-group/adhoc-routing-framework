#include <stdlib.h> 
#include <string.h>
#include "buffered_packet.h"

BufferedPacket::BufferedPacket(int portId, char* buffer, int length){
    this->portId = portId;
    this->buffer = (char *)(malloc(length));
    memcpy(this->buffer, buffer, length);
    this->length = length;
}

BufferedPacket::BufferedPacket(){
    this->portId = 0;
    this->length = 0;
    this->buffer = nullptr;
}

BufferedPacket::BufferedPacket(const BufferedPacket &bufferedPacket){
    this->portId = bufferedPacket.portId;
    this->buffer = bufferedPacket.getBuffer();
    this->length = bufferedPacket.length;
}

BufferedPacket::~BufferedPacket(){
    free(this->buffer);
}

BufferedPacket& BufferedPacket::operator=(const BufferedPacket& bufferedPacket){
    this->portId = bufferedPacket.portId;
    this->buffer = bufferedPacket.getBuffer();
    this->length = bufferedPacket.length;
    return *this;
}
        
int BufferedPacket::getPortId() const{
    return this->portId;
}

char* BufferedPacket::getBuffer() const{
    char* bufferCopy = (char *)(malloc(this->length));
    memcpy(bufferCopy, this->buffer, this->length);
    return bufferCopy;
}

int BufferedPacket::getLength() const{
    return this->length;
}