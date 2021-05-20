#ifndef HARDWARE_SINGLE_HOP_H
#define HARDWARE_SINGLE_HOP_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
#include "endpoint.h"
#include "tcp_socket.h"
#include "single_hop.h"
#include "hardware_defines.h"

class HardwareSingleHop : public SingleHop {
public:
	/*!
	 * @brief Construct a new HardwareAODV object
	 *
	 * @param ip the ip address of the current device
	 */
	HardwareSingleHop(uint32_t ip);
	HardwareSingleHop(const char* ip);
	~HardwareSingleHop();

	virtual int handlePackets() override;

protected:
	bool _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) override;
	void _buildPort(Port*) override;
	void _destroyPort(Port*) override;

	unordered_map<int, thread> portThreads;
	unordered_map<int, TCPSocket*> portSockets;

private:
	void _hardwareSH();
};

#endif
