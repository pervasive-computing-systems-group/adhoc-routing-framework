#include "hardware_aodv.h"
#include <stdlib.h>
#include <iostream>

// Helpers
void HardwareAODV::_hardwareAODV(){
    aodvSocket = new UDPSocket();
    const int REUSE_VAL = 1;
    if(!aodvSocket->init()){
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not initialize the aodv socket\n");
    }
    if(!aodvSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &REUSE_VAL, sizeof(REUSE_VAL))){
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not set the aodv socket to reuse ports\n");
    }
    if (!aodvSocket->bindToPort(ROUTING_PORT)) {
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not bind the aodv socket to port:%d\n", ROUTING_PORT);
    }
    if(!aodvSocket->setBroadcasting()){
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not set the aodv socket to broadcasting\n");
        exit(-1);
    }

    // create thread
    aodving = thread(&UDPSocket::receiveFromPortThread, aodvSocket);
    aodving.detach();
}

// Constructors
HardwareAODV::HardwareAODV():AODV(){
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
    if(HARDWARE_DEBUG){
        printf("[HARDWARE]:[DEBUG]: Destructing hardware aodv\n");
    }
    
    delete aodvSocket;

    int n = portSockets.size();
    while(n-- > 0){
        delete portSockets.begin()->second;
        portSockets.erase(portSockets.begin());
    }
}

// Public functions
int HardwareAODV::handlePackets(){
    int count = 0;
    Message message;
    // Handle aodv packets
    while(aodvSocket->getMessage(message)){
        char* messageData = message.getData();
        _handleAODVPacket(messageData, message.getLength(), message.getAddressI());
        free(messageData);
        count ++;
    }
    // Handle packets on the ports
    for(auto socketPair : portSockets){
        while(socketPair.second->getMessage(message)){
            char* messageData = message.getData();
            this->_handlePacket(socketPair.first, messageData, message.getLength(), message.getAddressI());
            free(messageData);
            count++;
        }
    }
    return count;
}

// Private Functions
int HardwareAODV::_socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest){
    if(portId == ROUTING_PORT){
        if(HARDWARE_DEBUG){
            printf("[HARDWARE]:[DEBUG]: sending packet '");
            printPacket(stdout, buffer, length);
            printf("' through AODV\n");
        }
        return ((Socket*)aodvSocket)->sendTo(buffer, length, dest, portId) > 0;
    }
    if(ports.count(portId)){
        if(HARDWARE_DEBUG){
            printf("[HARDWARE]:[DEBUG]: sending packet '");
            printPacket(stdout, buffer, length);
            printf("' through port %d\n", portId);
        }
        return portSockets[portId]->sendTo(buffer, length, dest, portId) > 0;
    }
    fprintf(stderr, "[HARDWARE]:[ERROR]: Tried to send packet '");
    printPacket(stderr, buffer, length);
    fprintf(stderr, "' through non-existent port %d\n", portId);
    return -1;
}

void HardwareAODV::_buildPort(Port* p){
    // Create socket
    UDPSocket* portSocket = new UDPSocket();
    const int REUSE_VAL = 1;
    if(!portSocket->init()){
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not initialize the aodv port socket\n");
        exit(-1);
    }
    if(!portSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &REUSE_VAL, sizeof(REUSE_VAL))){
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not set the aodv port socket to reuse ports\n");
        exit(-1);
    }
    if (!portSocket->bindToPort(p->getPortId())) {
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not bind the aodv port socket to port:%d\n", p->getPortId());
        exit(-1);
    }
    if(!portSocket->setBroadcasting()){
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not set the aodv port socket to broadcasting\n");
        exit(-1);
    }
    
    portSockets[p->getPortId()] = portSocket;

    // create thread
    portThreads[p->getPortId()] = thread(&UDPSocket::receiveFromPortThread, portSocket);
    portThreads[p->getPortId()].detach();
    
    if(UDP_DEBUG){
        printf("[HARDWARE]:[DEBUG]: Created port %d\n", p->getPortId());
    }
}

void HardwareAODV::_destroyPort(Port* p){
    if(UDP_DEBUG){
        printf("[HARDWARE]:[DEBUG]: Destroying port %d\n", p->getPortId());
    }
    if(portSockets.count(p->getPortId())){
        UDPSocket* portSocket = portSockets[p->getPortId()];
        portSockets.erase(p->getPortId());
        delete portSocket;
    }
}
