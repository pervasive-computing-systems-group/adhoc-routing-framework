#include "hardware_aodv.h"
#include <stdlib.h>
#include <iostream>

// Helpers
void HardwareAODV::_hardwareAODV(){
    if(this->addSocket(ROUTING_PORT)){
        this->aodvSocket = this->m_mSockets[ROUTING_PORT];
    }else{
        fprintf(stderr, "[AODV]:[ERROR]: Unable to build AODV port");
        exit(-1);
    }
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
}

/*
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
*/

// Private Functions
int HardwareAODV::_socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest){
    if(this->m_mSockets.count(portId)){
        if(HARDWARE_DEBUG){
            printf("[HARDWARE]:[DEBUG]: sending packet '");
            printPacket(stdout, buffer, length);
            printf("' through port %d\n", portId);
        }
        return this->m_mSockets[portId]->sendTo(buffer, length, dest, portId) > 0;
    }
    
    fprintf(stderr, "[HARDWARE]:[ERROR]: Tried to send packet '");
    printPacket(stderr, buffer, length);
    fprintf(stderr, "' through non-existent port %d\n", portId);
    return -1;
}

void HardwareAODV::_buildPort(Port* p){
    this->addSocket(p->getPortId());
}

void HardwareAODV::_destroyPort(Port* p){
    this->removeSocket(p->getPortId());
}

Socket* HardwareAODV::_protocolCreateSocket(uint32_t nPortNum, AppPacketHandler* pAppPacketHandler){
    // Create socket
    UDPSocket* newSocket = new UDPSocket();

    const int REUSE_VAL = 1;
    if(!newSocket->init()){
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not initialize the aodv port socket\n");
        exit(-1);
    }
    if(!newSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &REUSE_VAL, sizeof(REUSE_VAL))){
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not set the aodv port socket to reuse ports\n");
        exit(-1);
    }
    if (!newSocket->bindToPort(nPortNum)) {
        fprintf(stderr, "[HARDWARE]:[ERROR]: Could not bind the aodv port socket to port:%d\n", nPortNum);
        exit(-1);
    }
    if(!newSocket->setBroadcasting()){
        fprintf(stderr, "[HARDWARE]:[ERROR]:Could not set the aodv port socket to broadcasting\n");
        exit(-1);
    }

    // Set app packet handler
    newSocket->setAppPacketHandler(pAppPacketHandler);

    // create thread
    socketThreads[nPortNum] = thread(&UDPSocket::receiveFromPortThread, newSocket);
    socketThreads[nPortNum].detach();
    
    if(UDP_DEBUG){
        printf("[HARDWARE]:[DEBUG]: Created port %d\n", nPortNum);
    }

    return newSocket;
}

bool HardwareAODV::_protocolDestroySocket(uint32_t nPortNum){
    if(UDP_DEBUG){
        printf("[HARDWARE]:[DEBUG]: Destroying socket on port %d\n", nPortNum);
    }
    // TODO: clean up the memory
    // TODO: OPTIONAL: Stop the thread it is on
}
