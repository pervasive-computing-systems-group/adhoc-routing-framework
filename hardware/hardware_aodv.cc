#include "hardware_aodv.h"
#include <stdlib.h>
#include <iostream>

// Helpers
void HardwareAODV::_hardwareAODV(){
    //TODO: Only set aodv socket to broadcasting
    aodvSocket = new UDPSocket();
    int reuseVal = 1;
    if(!aodvSocket->init()){
        fprintf(stderr, "Could not initialize the aodv socket\n");
    }
    if(!aodvSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &reuseVal, sizeof(reuseVal))){
        fprintf(stderr, "Could not set the aodv socket to reuse ports\n");
    }
    if (!aodvSocket->bindToPort(AODV_PORT)) {
        fprintf(stderr, "Could not bind the aodv socket to port:%d\n", AODV_PORT);
    }
    if(!aodvSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the aodv socket to broadcasting\n");
        exit(-1);
    }

    dataSocket = new UDPSocket();
    if(!aodvSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &reuseVal, sizeof(reuseVal))){
        fprintf(stderr, "Could not set the data socket to reuse ports\n");
    }
    if (!dataSocket->bindToPort(DATA_PORT)) {
        fprintf(stderr, "Could not bind the data socket to port:%d\n", DATA_PORT);
    }
    if(!dataSocket->setBroadcasting()){
        fprintf(stderr, "Could not set the data socket to broadcasting\n");
        exit(-1);
    }
    
    // create threads 
    aodving = thread(&UDPSocket::receiveFromPortThread, aodvSocket);
    dataing = thread(&UDPSocket::receiveFromPortThread, dataSocket);

    aodving.detach();
    dataing.detach();
}

// Constructors
HardwareAODV::HardwareAODV(){
    _hardwareAODV();
}

HardwareAODV::HardwareAODV(uint32_t ip) : AODV(ip) {
    _hardwareAODV();
}

HardwareAODV::HardwareAODV(const char* ip) : AODV(ip) {
    _hardwareAODV();
}

// Destructors
HardwareAODV::~HardwareAODV(){
    aodving.~thread();
    dataing.~thread();
}

// Override functions
int HardwareAODV::socketSendPacket(char *buffer, int length, IP_ADDR dest, int port){
    if(port == AODV_PORT){
        if(HARDWARE_DEBUG){
            printf("[DEBUG]: sending packet ");
            printPacket(stdout, buffer, length);
            printf(" through AODV\n");
        }
        return aodvSocket->sendTo(buffer, length, dest, port);
    }else{
        if(HARDWARE_DEBUG){
            printf("[DEBUG]: sending packet ");
            printPacket(stdout, buffer, length);
            printf(" through DATA\n");
        }
        return dataSocket->sendTo(buffer, length, dest, port);
    }
}

// Class methods
int HardwareAODV::handlePackets(){
    Message message;
    int count = 0;
    int count_data = 0;
    if(HARDWARE_DEBUG) printf("[DEBUG]: Handling received packets from AODV\n");
    while(aodvSocket->getMessage(message)){
        if(HARDWARE_DEBUG){
            printf("[DEBUG]: Received: %s", message.getData());
            printPacket(stdout, message.getData(), message.getLength());
            printf("\n");
        }
        decodeReceivedPacketBuffer(message.getData(), message.getLength(), message.getAddressI());
        count ++;
    }
    if(HARDWARE_DEBUG) printf("[DEBUG]: Handled %d packets from AODV\n", count);
    
    if(HARDWARE_DEBUG) printf("[DEBUG]: Handling received packets from DATA\n");
    while(dataSocket->getMessage(message)){
        if(HARDWARE_DEBUG){
            printf("[DEBUG]: Received: %s", message.getData());
            printPacket(stdout, message.getData(), message.getLength());
            printf("\n");
        }
        decodeReceivedPacketBuffer(message.getData(), message.getLength(), message.getAddressI());
        count ++;
        count_data ++;
    }
    if(HARDWARE_DEBUG) printf("[DEBUG]: Handled %d packets from DATA\n", count_data);

    return (count == 0) ? -1 : count;
}



void printPacket(FILE* file, char * buffer, int length){
    for(int i = 0; i < length; i++){
        fprintf(file, "%c", buffer[i]);
    }
}