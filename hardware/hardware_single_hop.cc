#include "hardware_single_hop.h"
#include <stdlib.h>
#include <iostream>

/******************************
 * Constructors + Destructors
 ******************************/

HardwareSingleHop::HardwareSingleHop(uint32_t ip) : SingleHop(ip) {
	_hardwareSH();
}

HardwareSingleHop::HardwareSingleHop(const char* ip) : SingleHop(ip) {
	_hardwareSH();
}

HardwareSingleHop::~HardwareSingleHop() {
	if(HARDWARE_DEBUG) {
		printf("[HARDWARE]:[DEBUG]: Destructing hardware aodv\n");
	}

	// Free sockets
	for(auto& it: portSockets) {
		delete it.second;
	}
}


/******************************
 * Public Functions
 ******************************/

int HardwareSingleHop::handlePackets() {
	int count = 0;
	Message message;

	// Handle packets on the ports
	for(auto socketPair : portSockets) {
		while(socketPair.second->getMessage(message)) {
			char* messageData = message.getData();
			this->_handlePacket(socketPair.first, messageData, message.getLength(), message.getAddressI());
			free(messageData);
			count++;
		}
	}

	return count;
}


/******************************
 * Private Functions
 ******************************/

bool HardwareSingleHop::_socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) {
	bool ret_val = false;

	// If port exists, send packet
	if(ports.count(portId)) {
		if(HARDWARE_DEBUG){
			printf("[HARDWARE]:[DEBUG]: sending packet '");
			printPacket(stdout, buffer, length);
			printf("' through port %d\n", portId);
		}

		ret_val = (portSockets[portId]->sendTo(buffer, length, dest, portId) > 0);
	}
	else {
		fprintf(stderr, "[HARDWARE]:[ERROR]: Tried to send packet '");
		printPacket(stderr, buffer, length);
		fprintf(stderr, "' through non-existent port %d\n", portId);
	}

	return ret_val;
}

void HardwareSingleHop::_buildPort(Port* p) {
	// Create socket
	TCPSocket* portSocket = new TCPSocket();
	const int REUSE_VAL = 1;
	if(!portSocket->init()){
		fprintf(stderr, "[HARDWARE]:[ERROR]: Could not initialize the tcp port socket\n");
		exit(-1);
	}
	if(!portSocket->setOption(SOL_SOCKET, SO_REUSEPORT, &REUSE_VAL, sizeof(REUSE_VAL))){
		fprintf(stderr, "[HARDWARE]:[ERROR]: Could not set the tcp port socket to reuse ports\n");
		exit(-1);
	}
	if (!portSocket->bindToPort(p->getPortId())) {
		fprintf(stderr, "[HARDWARE]:[ERROR]: Could not bind the tcp port socket to port:%d\n", p->getPortId());
		exit(-1);
	}
	if(!portSocket->setBroadcasting()){
		fprintf(stderr, "[HARDWARE]:[ERROR]:Could not set the tcp port socket to broadcasting\n");
		exit(-1);
	}

	portSockets[p->getPortId()] = portSocket;

	// create thread
	portThreads[p->getPortId()] = thread(&TCPSocket::receiveFromPortThread, portSocket);
	portThreads[p->getPortId()].detach();

	if(TCP_DEBUG){
		printf("[HARDWARE]:[DEBUG]: Created port %d\n", p->getPortId());
	}
}

void HardwareSingleHop::_destroyPort(Port* p){
	if(TCP_DEBUG){
		printf("[HARDWARE]:[DEBUG]: Destroying port %d\n", p->getPortId());
	}
	if(portSockets.count(p->getPortId())){
		TCPSocket* portSocket = portSockets[p->getPortId()];
		portSockets.erase(p->getPortId());
		delete portSocket;
	}
}



/******************************
 * Helper Functions
 ******************************/

void HardwareSingleHop::_hardwareSH() { }