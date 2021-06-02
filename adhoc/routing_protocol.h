#ifndef ROUTINGPROTOCOL_H
#define ROUTINGPROTOCOL_H

/********************************
 * RoutingProtocol.h 
 * 
 * Simple routing protocol base class for implementation of more complex routing protocols.
 * Contains necessary building blocks for custom routing protocols. 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

#include "adhoc_defines.h"
#include "port.h"
#include "../socket/socket.h"
#include "../socket/message.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map> 
#include <map>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include <mutex>
#include <unordered_map>

using namespace std;

// Classes that aren't built yet but will be
class Port;

// get a 32 bit ip address from a string
IP_ADDR getIpFromString(string ipStr);
string getStringFromIp(IP_ADDR ip);

/* Routing Table row class 
 * This class exists to make future modifications to priority in routing protocols */
class TableInfo 
{
public:
	IP_ADDR dest; // target destination 
	IP_ADDR nextHop; // next hop 

	uint16_t ttl; // time to live for this table entry 
};


/* Routing Table class */
class RoutingTable{
public:
	// default constructor 
	RoutingTable();
	virtual ~RoutingTable();

	const uint16_t DEFAULT_TTL = 1800; // 1800 seconds = 30 minutes 

	// returns the ip address of the next hop 
	// returns 0 if not in table 
	virtual IP_ADDR getNextHop(const IP_ADDR dest);

	// adds an element to the table 
	virtual void updateTableEntry(const IP_ADDR dest, const IP_ADDR nextHop);

	// get the map 
	map<IP_ADDR, TableInfo> getInternalTable() { return this->table; }

protected:
	// internal routing table maps destination to next hop 
	map<IP_ADDR, TableInfo> table;

};

static mutex globalMux;

/* Routing Protocol base class */
class RoutingProtocol {
public: 
	// default constructor
	RoutingProtocol();
	RoutingProtocol(IP_ADDR nIP);
    // Destructor
    virtual ~RoutingProtocol();

    // Functions
    /**
     * @brief adds a port to the routing protocol and sets the ports adhocRouting member
     * 
     * @param p the port to add
     */
    void addPort(Port* p);
    
    /**
     * @brief removes a port from the routing protocol
     * 
     * @param p the port to remove
     */
    void removePort(Port* p);

    /**
     * @brief Adds a data socket to the routing protocol
     * 
     * @param nPortNum The port for the socket
     * @param pAppPacketHandler The sockets packet handler
     * @return true if the socket was successfully added
     * @return false unable to add teh socket
     */
    bool addSocket(uint32_t nPortNum, AppPacketHandler* pAppPacketHandler = nullptr);
    
    /**
     * @brief Removes the socket from the routing protocl
     * 
     * @param nPortNum which port the socket to remove is on
     * @return true if the socket was removed successfully
     * @return false if the socket didn't exist or couldnt be removed
     */
    bool removeSocket(uint32_t nPortNum);

	/**
     * @brief Send a packet to a given ip address using a specified port
     * 
     * @param p the port to use
     * @param data the data to send
     * @param length the length of the data
     * @param dest 
     * @param origIP 
     * 
     * @returns The bytes sent or -1 on failure
     */
    int sendPacket(Port* p, char* data, int length, IP_ADDR dest, IP_ADDR origIP = -1);

	/**
     * @brief Send a packet to a given ip address using a specified port
     *
     * @param p the port to use
     * @param data the data to send
     * @param length the length of the data
     * @param dest
     * @param origIP
     *
     * @returns The bytes sent or -1 on failure
     */
    int sendPacket(int portId, char* data, int length, IP_ADDR dest, IP_ADDR origIP = -1);

    // Virtual Functions

    /**
     * @brief Handles the receiving or processing of all packets
     * @brief when implementing this should query each of the sockets corresponding to each port
     * @brief and then "give" the data to each port
     * TODO: By creating a Socket base class we could implement this code and avoid the above req. 
     * 
     * @returns the number of handled packets
     */
    virtual int handlePackets();
	
    /**
     * @brief is there a link between this node and dest? 
     * 
     * @param dest ip address of destination
     * @return true link exits
     * @return false link does not exist
     */
	virtual bool linkExists(IP_ADDR dest);
    /**
     * @brief function to reset the neighbors of this node to none 
     * 
     */
	void resetLinks();
    /**
     * @brief add this ip address to this list of current 1 hop neighbors 
     * 
     * @param node ip address to add
     */
    // TODO: Make add link a boolean
	void addLink(IP_ADDR node);
    
	
	// Getters and Setters
    /**
     * @brief Get the Ip object
     * 
     * @return const uint32_t 
     */
	const uint32_t getIp() { return ipAddress; }
    /**
     * @brief Set the Ip object
     * 
     * @param ip 
     */
	void setIp(const uint32_t ip) { ipAddress = ip; }

    // Mutex for if the neighbor states are properly updated 
    // Network monitoring objects use this mutex when updating a routing protocol's neighbors
    mutex neighborMux;

protected:
	// vector of one hop neighbors to this node. Can be from network monitoring, HELLO messages, etc
	vector<IP_ADDR> m_neighbors;
	// The ip address of the cimputer this routing protocol is running on
	uint32_t ipAddress;
	// The list of ports that can send and receive messages using this routing protocol
	// TODO: remove this and use m_mSockets to track sockets with port numbers (an integer)
	unordered_map<int, Port*> ports;

	// TODO: Move this into AODV, along with link related functions
	RoutingTable* m_pRoutingTable;
	// Map of Sockets held by the node
	unordered_map<uint32_t, Socket*> m_mSockets;

	// Functions
	virtual Socket* _protocolCreateSocket(uint32_t nPortNum, AppPacketHandler* pAppPacketHandler) = 0;
	virtual bool _protocolDestroySocket(uint32_t nPortNum) = 0;
	virtual void _buildPort(Port*) = 0;
    virtual void _destroyPort(Port*) = 0;

	// Handle the packet for specific routing protocols
	virtual void protocolHandlePacket(Socket* pSocket, Message* pMsg) = 0;

	/**
     * @brief Send a packet to a given ip address using a specified port
     *
     * @param p the port to use
     * @param data the data to send
     * @param length the length of the data
     * @param dest
     * @param origIP
     *
     * @returns The number of bytes sent
     */
    virtual int protocolSendPacket(int portId, char* data, int length, IP_ADDR dest, IP_ADDR origIP = -1) = 0;
};

#endif
