#include "single_hop.h"
#include "string.h"
#include <chrono>
#include <fstream>
#include <stdio.h>
#include <thread>

/******************************
 * Constructors + Destructors
 ******************************/

SingleHop::SingleHop(IP_ADDR ip, uint32_t nDataPort)
		: RoutingProtocol(ip),
		  m_nDataPort(nDataPort)
{
    if(SINGLE_HOP_DEBUG)
        cout << "[SH]:[DEBUG]: Created new single-hop routing protocol." << endl;
}

SingleHop::~SingleHop() { }

/******************************
 * Public Functions
 ******************************/
int SingleHop::protocolSendPacket(int portId, char* packet, int length, IP_ADDR dest, IP_ADDR origIP) {
	int ret_val = -1;
	if (SINGLE_HOP_DEBUG) {
		cout << "[SINGLE HOP]:[DEBUG]: Attempting to send packet from "
				<< getStringFromIp(getIp()) << " to "
				<< getStringFromIp(dest) << endl;
	}

	// Add header to buffer
	char *buffer = (char *)(malloc(HEADER_SIZE + length));
	// Configure header
	uint8_t zero = 0x00;
	memcpy((buffer + HEADER_LEAD_BYTE), &(zero), 1);
	memcpy((buffer + HEADER_DESTINATION), &dest, 4);
	memcpy((buffer + HEADER_ORIGIN), &origIP, 4);
	// Copy packet data into buffer
	memcpy((buffer + HEADER_SIZE), packet, length);

	// Attempt to send packet
	ret_val = _socketSendPacket(portId, buffer, length + HEADER_SIZE, dest);

	// TODO: Determine how we will handle packets that failed to send!

	free(buffer);

	return ret_val;
}


/******************************
 * Protected Functions
 ******************************/

void SingleHop::_handlePacket(int port, char *packet, int length, IP_ADDR source) {
	// Get final destination
	IP_ADDR finalDestination;
	memcpy(&finalDestination, &(packet[1]), 4);

	// Determine if this packet was ment for us (if not, something got weird...)
	if (this->getIp() == finalDestination || finalDestination == getIpFromString(BROADCAST_STR)) {
		// We are the final destination
		if(ports.count(port)) {
			if(SINGLE_HOP_DEBUG)
				std::cout << "[SINGLE HOP]:[INFO]: Received packet on port " << port << endl;

			// Pass packet onto socket handler
			ports[port]->handlePacket((packet + HEADER_SIZE), length - HEADER_SIZE, source);
		}
		else {
			fprintf(stderr, "[SINGLE HOP]:[ERROR]: Received packet on port with no port handler\n");
		}
	}
	else {
		fprintf(stderr, "[SINGLE HOP]:[ERROR]: Received packet for different destination...\n");
	}
}

// Handle the packet for single-hop
void SingleHop::protocolHandlePacket(Socket* pSocket, Message* pMsg) {

}

void SingleHop::_handlePacket(Port* p, char *buffer, int length, IP_ADDR source){
	_handlePacket(p->getPortId(), buffer, length, source);
}

int SingleHop::_socketSendPacket(Port* p, char *buffer, int length, IP_ADDR dest){
    return _socketSendPacket(p->getPortId(), buffer, length, dest);
}

// Deprecated function
void SingleHop::_buildPort(Port*) { }

// Deprecated function
void SingleHop::_destroyPort(Port*) { }

/******************************
 * Helper Functions
 ******************************/
