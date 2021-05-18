#ifndef SINGLE_HOP_H
#define SINGLE_HOP_H

/*********************************
 * single_hop.h
 *
 * Custom c++ implementation of a basic single-hop routing protocol.
 * 
 * Author: Jonathan Diller
 * Date: 5/10/21
 ********************************/

#include "routing_protocol.h"
#include "single_hop_defines.h"
#include "safe_circular_queue.h"

class SingleHop : public RoutingProtocol
{
public:
	// default constructor 
	SingleHop();
	SingleHop(const char* ip);
	SingleHop(IP_ADDR ip);
	~SingleHop();

	/**
     * @brief Send a packet to a given ip address using a specified port
     * 
     * @param portId the port number to use
     * @param packet the packet to send
     * @param length the length of the packet
     * @param dest the destination ip address
     * @param origIP IP address where the packet was from
     */
    virtual bool sendPacket(int portId, char* packet, int length, IP_ADDR dest, IP_ADDR origIP = -1) override;

protected:
	// Functions
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
	 * @brief Takes in a packet and routes it in the network or to the desired port
	 * 
	 * @param port the port class to send from/to
	 * @param buffer the data to be sent, needs to contain the AODV header
	 * @param length the length of that data
	 * @param source the ip address the packet was received from
	 */
	void _handlePacket(Port* p, char *buffer, int length, IP_ADDR source);

	// Send the data over a socket
	bool _socketSendPacket(Port* port, char *buffer, int length, IP_ADDR dest);

	// Virtual Functions
	virtual bool _socketSendPacket(int portId, char *buffer, int length, IP_ADDR dest) = 0;
};

#endif
