#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
#include "endpoint.h"
#include "tcp_ap_socket.h"
#include "single_hop.h"
#include "hardware_defines.h"

class HardwareSHAP : public SingleHop {
public:
	/*!
	 * @brief Construct a new HardwareAODV object
	 *
	 * @param nIP: the IP address of the current device
	 * @param nDataPortNum: the data port number
	 * @param pDataAPH: a pointer to an AppPacketHandler for packets received on the data socket
	 */
	HardwareSHAP(const char* nIP, uint32_t nDataPortNum, AppPacketHandler* pDataAPH = NULL);
	HardwareSHAP(uint32_t nIP, uint32_t nDataPortNum, AppPacketHandler* pDataAPH = NULL);
	~HardwareSHAP();

protected:
	int _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) override;

	unordered_map<int, thread> socketThreads;

private:
	// Shared private constructor
	void _hardwareSHAP(uint32_t nDataPortNum, AppPacketHandler* pDataAPH);

	// Thread to listen for incoming socket requests
	thread m_oListeningThread;
};
