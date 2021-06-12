#include "hardware_sh_ap.h"
#include <stdlib.h>
#include <iostream>

/******************************
 * Constructors + Destructors
 ******************************/

HardwareSHAP::HardwareSHAP(const char* nIP, uint32_t nDataPortNum, AppPacketHandler* pDataAPH)
		: SingleHop(getIpFromString(nIP), nDataPortNum)
{
	_hardwareSHAP(nDataPortNum, pDataAPH);
}

HardwareSHAP::HardwareSHAP(uint32_t nIP, uint32_t nDataPortNum, AppPacketHandler* pDataAPH)
		: SingleHop(nIP, nDataPortNum)
{
	_hardwareSHAP(nDataPortNum, pDataAPH);
}

HardwareSHAP::~HardwareSHAP() {
	if(HARDWARE_DEBUG) {
		printf("[HARDWARE]:[DEBUG]: Destructing hardware aodv\n");
	}

	// Delete listening socket
	auto listSoc = m_mSockets.find(this->m_nDataPort);
	if(listSoc != m_mSockets.end()) {
		listSoc->second->sclose();
		delete listSoc->second;
		m_mSockets.erase(listSoc);
	}
}


/******************************
 * Public Functions
 ******************************/


/******************************
 * Protected Functions
 ******************************/

int HardwareSHAP::_socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) {
	// This function is not currently needed for the access point, it isn't sending packets out
	return -1;
}


/******************************
 * Private Functions
 ******************************/

// Shared private constructor
void HardwareSHAP::_hardwareSHAP(uint32_t nDataPortNum, AppPacketHandler* pDataAPH) {
	// Listening socket used to accept new clients
	TCPAPSocket *listeningSocket = new TCPAPSocket(nDataPortNum);
	// Add application handler
	listeningSocket->setAppPacketHandler(pDataAPH);
	// Add application connection handler
	listeningSocket->setAppConnectionHandler(this->m_pAppConnectionHandler);
	// Add listener to socket map
	m_mSockets.insert(std::pair<uint32_t, Socket*>(nDataPortNum, listeningSocket));

	// Add listening thread
	m_oListeningThread = thread(&TCPAPSocket::checkListeningPort, listeningSocket);
	m_oListeningThread.detach();
}
