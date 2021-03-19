#include "message.h"

Message::Message(Endpoint end, char* data, int length){
    this->end = end;
    this->data = (char *) malloc(length);
    memcpy(this->data, data, length);
    this->length = length;
}

Message::Message(const Message &other){
    this->end = other.end;
    this->data = other.getData();
    this->length = other.length;
}

Message::~Message(){
    free(this->data);
}

Message& Message::operator=(const Message& other){
    this->end = other.end;
    this->data = other.getData();
    this->length = other.length;
    return *this;
}

uint32_t  Message::getAddressI(void) const{
    return end.getAddressI();
}


char* Message::getAddressC(void){
    return end.getAddressC();
}


int Message::getLength(void) const{
    return length;
}


char* Message::getData(void) const{
    char* dataCopy = (char *) malloc(this->length);
    memcpy(dataCopy, this->data, this->length);
    return dataCopy;
}


int Message::getPort(void) const{
    return end.getPort();
}


Endpoint& Message::getEndpoint(void){
    return end;
}
