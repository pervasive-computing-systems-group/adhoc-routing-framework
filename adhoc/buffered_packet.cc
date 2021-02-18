#include <stdlib.h> 
#include <string.h>
#include "buffered_packet.h"

BufferedPacket::BufferedPacket(int portId, char* buffer, int length){
    this->portId = portId;
    this->buffer = (char *)(malloc(length));
    memcpy(this->buffer, buffer, length);
    this->length = length;
}

BufferedPacket::~BufferedPacket(){
    free(this->buffer);
}
        
int BufferedPacket::getPortId(){
    return this->portId;
}

char* BufferedPacket::getBuffer(){
    char* bufferCopy = (char *)(malloc(this->length));
    memcpy(bufferCopy, this->buffer, this->length);
    return bufferCopy;
}

int BufferedPacket::getLength(){
    return this->length;
}