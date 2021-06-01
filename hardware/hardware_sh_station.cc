#include "hardware_sh_station.h"
#include <stdlib.h>
#include <iostream>

/******************************
 * Constructors + Destructors
 ******************************/

HardwareSHStation::HardwareSHStation(const char* nIP, uint32_t nDataPortNum, AppPacketHandler* pDataAPH)
		: SingleHop(getIpFromString(nIP), nDataPortNum)
{
	_hardwareSHAP(nDataPortNum, pDataAPH);
}

HardwareSHStation::HardwareSHStation(uint32_t nIP, uint32_t nDataPortNum, AppPacketHandler* pDataAPH)
		: SingleHop(nIP, nDataPortNum)
{
	_hardwareSHAP(nDataPortNum, pDataAPH);
}

HardwareSHStation::~HardwareSHStation() {
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

bool HardwareSHStation::_socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) {
	bool ret_val = false;

	// Call send packet on data socket
	auto soc = m_mSockets.find(portId);
	if(soc != m_mSockets.end()) {
		int bytesSent = soc->second->sendTo(buffer, length, dest, portId);
		// TODO: Continue here!!
		if(bytesSent < 0) {
			if(HARDWARE_DEBUG) {
				printf("[HARDWARE]:[HardwareSHStation]:[DEBUG]: Failed to send packet\n");
			}

			ret_val = false;
		}
		else {
			if(HARDWARE_DEBUG) {
				printf("[HARDWARE]:[HardwareSHStation]:[DEBUG]: Sent %d bytes\n", bytesSent);
			}

			ret_val = true;
		}
	}
	else {
		if(HARDWARE_DEBUG) {
			printf("[HARDWARE]:[HardwareSHStation]:[DEBUG]: Failed to find socket in _socketSendPacket()\n");
		}

		ret_val = false;
	}

	return ret_val;
}


/******************************
 * Private Functions
 ******************************/

// Shared private constructor
void HardwareSHStation::_hardwareSHAP(uint32_t nDataPortNum, AppPacketHandler* pDataAPH) {
	// Listening socket used to accept new clients
	TCPStationSocket *sendSocket = new TCPStationSocket(nDataPortNum);
	// Add application handler
	// TODO: make send handlers
//	sendSocket->setAppPacketHandler(pDataAPH);
	// Add listener to socket map
	m_mSockets.insert(std::pair<uint32_t, Socket*>(nDataPortNum, sendSocket));
}
