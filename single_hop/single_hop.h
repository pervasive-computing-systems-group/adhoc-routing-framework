/*********************************
 * single_hop.h
 *
 * Custom c++ implementation of a basic single-hop routing protocol.
 * 
 * Author: Jonathan Diller
 * Date: 5/10/21
 ********************************/

#pragma once

#include "routing_protocol.h"
#include "single_hop_defines.h"
#include "safe_circular_queue.h"

class SingleHop : public RoutingProtocol
{
public:
	/**
     * @brief SingleHop constructor
     *
     * @param nIP: the IP address of this device
     * @param nDataPort: the data port number
     */
	SingleHop(IP_ADDR nIP, uint32_t nDataPort);
	~SingleHop();

protected:
	/// Functions
	/**
	 * @brief Takes in a packet and routes it in the network or to the desired port
	 * 
	 * @param portId the port id to send from/to
	 * @param buffer the data to be sent, needs to contain the AODV header
	 * @param length the length of that data
	 * @param source the ip address the packet was received from
	 */
	virtual void _handlePacket(int portId, char *buffer, int length, IP_ADDR source);

	/**
     * @brief Send a packet to a given ip address using a specified port
     *
     * @param portId the port number to use
     * @param packet the packet to send
     * @param length the length of the packet
     * @param dest the destination ip address
     * @param origIP IP address where the packet was from
     */
    virtual int protocolSendPacket(int portId, char* packet, int length, IP_ADDR dest, IP_ADDR origIP = -1) override;

	// Handle the packet for single-hop
	virtual void protocolHandlePacket(Socket* pSocket, Message* pMsg) override;

	/**
	 * @brief Takes in a packet and routes it in the network or to the desired port
	 * 
	 * @param port the port class to send from/to
	 * @param buffer the data to be sent, needs to contain the AODV header
	 * @param length the length of that data
	 * @param source the ip address the packet was received from
	 */
	void _handlePacket(Port* p, char *buffer, int length, IP_ADDR source);

	// Send the data over a socket
	int _socketSendPacket(Port* port, char *buffer, int length, IP_ADDR dest);

	// Virtual Functions
	virtual int _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) = 0;

	// Deprecated function
	void _buildPort(Port*) override;
	// Deprecated function
	void _destroyPort(Port*) override;
	// TODO: Implement these
	Socket* _protocolCreateSocket(uint32_t nPortNum, AppPacketHandler* pAppPacketHandler) override {return nullptr;};
	virtual bool _protocolDestroySocket(uint32_t nPortNum) override {return false;};

	/// Member variables
	// The data port number. All data (and foreseeable communication) occurs over just one
	// port in the single-hop approach.
	const uint32_t m_nDataPort;
};
