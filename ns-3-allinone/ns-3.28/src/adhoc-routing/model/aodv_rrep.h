#ifndef AODVRREP_H
#define AODVRREP_H
/*********************************
 * aodv_rrep.h
 *
 * Custom c++ implementation of the aodv rrep message 
 * 
 * Author: Josh Rands
 * Date: 9/19/2019
 ********************************/

#include "aodv_defines.h"
#include "aodv_params.h"
#include "aodv_packets.h"
#include "aodv_routing_table.h"

class RREPHelper 
{
public: 
	// default constructor
	RREPHelper();
	RREPHelper(IP_ADDR ip, AODVRoutingTable* table, uint32_t* seqNum);

	void setIp(const IP_ADDR ip) { this->m_ip = ip; }
	void setRoutingTable(AODVRoutingTable* table) { this->m_pTable = table; }
	void setSequenceNum(uint32_t* seqNum) { this->m_pSequenceNum = seqNum; }

	// initiating RREP enters state of waiting for RREP - source is to update routing table on reverse route 
	rrepPacket createRREPFromRREQ(rreqPacket rreq, IP_ADDR source);
	// forward RREP enters state of maybe receiving RREP
	rrepPacket createForwardRREP(const rrepPacket& receivedRREP, IP_ADDR source);
	// convert rrep message to a char* buffer
	static char* createRREPBuffer(const rrepPacket& rrep);
	// read a received rrep buffer
	static rrepPacket readRREPBuffer(char* buffer);

private:
    IP_ADDR m_ip;
    AODVRoutingTable* m_pTable;
	uint32_t* m_pSequenceNum;
};

#endif
