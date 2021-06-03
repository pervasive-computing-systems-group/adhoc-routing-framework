#ifndef HARDWAREAODV_H
#define HARDWAREAODV_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>
#include "endpoint.h"
#include "udp_socket.h"
#include "aodv.h"
#include "hardware_defines.h"

class HardwareAODV : public AODV{
private:
    void _hardwareAODV();
    
protected:
    unordered_map<int, thread> socketThreads;
    Socket* aodvSocket;
    thread aodving;

public:
    // Constructors

    HardwareAODV();

    /*!
     * @brief Construct a new HardwareAODV object
     * 
     * @param ip the ip address of the current device
     */
    HardwareAODV(uint32_t ip);
    HardwareAODV(const char* ip);

    // Destructors
    ~HardwareAODV();

protected:
    int _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) override;
    // Depreciating
	void _buildPort(Port*) override;
    void _destroyPort(Port*) override;
    // TODO: Implement these:
	Socket* _protocolCreateSocket(uint32_t nPortNum, AppPacketHandler* pAppPacketHandler) override;
	virtual bool _protocolDestroySocket(uint32_t nPortNum) override;
};


#endif
