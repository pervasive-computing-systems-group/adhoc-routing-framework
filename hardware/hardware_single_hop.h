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
private:
    void _hardwareSH();

protected:
    unordered_map<int, thread> portThreads;
    unordered_map<int, TCPSocket*> portSockets;

public:
    // Constructors

    HardwareSingleHop();

    /*!
     * @brief Construct a new HardwareAODV object
     *
     * @param ip the ip address of the current device
     */
    HardwareSingleHop(uint32_t ip);
    HardwareSingleHop(const char* ip);

    // Destructors
    ~HardwareSingleHop();

    virtual int handlePackets() override;

protected:
    bool _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) override;
	void _buildPort(Port*) override;
    void _destroyPort(Port*) override;
};

/*!
 * @brief prints the data of a packet to the specified filed
 *
 * @param file  where to print the packet
 * @param buffer the packet to print
 * @param length the length of the packet
 */
//void printPacket(FILE* file, char * buffer, int length);


#endif
