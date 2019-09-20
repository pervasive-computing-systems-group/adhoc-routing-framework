#include "aodv.h"
#include "string.h"
#include "send_packet.h"
#include <fstream>
#include <stdio.h>

AODV::AODV(IP_ADDR ip)
{
//	cout << "Created new aodv routing protocol." << endl;

	this->ipAddress = ip;
	this->sequenceNum = 0;
	this->table = new AODVRoutingTable();

	this->rreqHelper.setRoutingTable((AODVRoutingTable*)(this->table));
	this->rreqHelper.setIp(ip);
	this->rreqHelper.setSequenceNumPointer(&(this->sequenceNum));

	this->rrepHelper.setIp(ip);
	this->rrepHelper.setRoutingTable((AODVRoutingTable*)(this->table));
	this->rrepHelper.setSequenceNum(&(this->sequenceNum));
}

AODV::~AODV()
{
	delete this->table;
}

void AODV::sendPacketBuffer(char* packet, int length, IP_ADDR finalDestination)
{
	// if entry exists in routing table, send it! 
	// check routing table 
	IP_ADDR nextHop = this->getTable()->getNextHop(finalDestination);
	if (0 == nextHop)
	{
		// start a thread for an rreq and wait for a response 
		rreqPacket rreq = this->rreqHelper.createRREQ(finalDestination);

		broadcastRREQBuffer(rreq);
	}

	// add aodv header to buffer 
	char* buffer = (char*)(malloc(5 + length));
	uint8_t zero = 0x00;
	memcpy(buffer, &(zero), 1);
	buffer++;
	memcpy(buffer, &finalDestination, 4);
	buffer+=4;

	// copy data into packet 
	memcpy(buffer, packet, length);
	// reset packet 
	packet-=5;

	sendBuffer(buffer, length+5, this->getIp(), nextHop);
}

void AODV::broadcastRREQBuffer(rreqPacket rreq)
{
	char* rreqBuffer = RREQHelper::createRREQBuffer(rreq);
	sendBuffer(rreqBuffer, sizeof(rreq), this->getIp(), getIpFromString(BROADCAST));

	delete rreqBuffer;
}

void AODV::decodeReceivedPacketBuffer(char* buffer, int length, IP_ADDR source)
{
	if (length <= 0)
	{
		cout << "ERROR DECODING PACKET. Length = 0." << endl;
	}

	// determine type of message 
	uint8_t type;
	memcpy(&type, buffer, 1);

	switch (type)
	{
		case 1:
			handleRREQ(buffer, length, source);
			break;
		case 2: 
			handleRREP(buffer, length, source);
			break;
		case 3:
			handleRERRBuffer(buffer, length);
			break;
		default:
			if (AODV_DEBUG)
				cout << "ERROR: Packet not AODV." << endl;
			break;
	}
}

void AODV::handleRREQ(char* buffer, int length, IP_ADDR source)
{
	// handle a received rreq message

	// 1. make sure this is a valid rreq message 
	if (length != sizeof(rreqPacket))
	{
		if (AODV_DEBUG)
			cout << "ERROR handling rreq packet. Invalid length." << endl;

		return;
	}

	// valid rreq packet, make a decision
	rreqPacket rreq = rreqHelper.readRREQBuffer(buffer);

	// 2. is this a duplicate rreq? 
	if (rreqHelper.isDuplicateRREQ(rreq))
	{
		if (AODV_DEBUG)
			cout << "Duplicate RREQ message received." << endl;

		return;
	}

	// 3. should we generate a rrep? are we the final destination?
	if (rreqHelper.shouldGenerateRREP(rreq))
	{
		// generate a rreq message from this rreq
		// TODO: Implement this 
		if (RREP_DEBUG)
			cout << "Generating RREP message..." << endl;
		rrepPacket rrep = rrepHelper.createRREPFromRREQ(rreq,source);

		// convert packet to buffer and send 
		char* buffer;
		buffer = RREPHelper::createRREPBuffer(rrep);

		sendBuffer(buffer, sizeof(rrep), this->getIp(), rrep.origIP);

		return;
	}

	// 4. not final destination, forward the rreq 
	rreqPacket forwardRREQ = rreqHelper.createForwardRREQ(rreq, source);
	broadcastRREQBuffer(forwardRREQ);
}

void AODV::handleRREP(char* buffer, int length, IP_ADDR source)
{
	// handle a received rrep message

	// 1. make sure this is a valid rrep message 
	if (length != sizeof(rrepPacket))
	{
		if (AODV_DEBUG)
			cout << "ERROR handling rrep packet. Invalid length." << endl;

		return;
	}

	// valid rreq packet, make a decision
	rrepPacket rrep = rrepHelper.readRREPBuffer(buffer);

	// 2. are with the original ip? was this our route request reply? 
    if (this->getIp() == rrep.origIP)
    {
        // packet made it back! 
        if (AODV_DEBUG)
            cout << "Route discovery complete!" << endl;

        this->getTable()->updateAODVRoutingTableFromRREP(&rrep,source);
    }
    else 
    {
        // forward this packet 
		rrepPacket forwardRREP = this->rrepHelper.createForwardRREP(rrep, source);

        IP_ADDR nextHopIp = this->getTable()->getNextHop(forwardRREP.origIP);
        char* buffer = RREPHelper::createRREPBuffer(forwardRREP);
		sendBuffer(buffer, sizeof(forwardRREP), this->getIp(), nextHopIp);

		delete buffer;
    }
}

void AODV::handleRERRBuffer(char* buffer, int length)
{
	
}

RERR createRERR(const IP_ADDR dest)
{

}

void forwardRERR(const RERR receivedRERR)
{

}

char* createRERRBuffer(const RERR rerr)
{

}

RERR readRERRBuffer(char* buffer)
{

}

void AODV::logRoutingTable()
{
	ofstream logFile;
	logFile.open("./logs/" + getStringFromIp(this->getIp()) + "-log.txt", ios::out);

	if (logFile.is_open())
		logFile << "AODV Log for node " << this->getIp() << endl;

	// check if there are any entries 
	if (this->getTable()->getInternalTable().size() == 0)
	{
		logFile << "Routing table empty." << endl;
		return;
	}

	map<IP_ADDR, TableInfo>::iterator it;

	logFile << "DESTINATION IP : NEXT HOP" << endl;

	for ( it = this->getTable()->getInternalTable().begin(); it != this->getTable()->getInternalTable().end(); it++ )
	{
		logFile << getStringFromIp(it->first)
				<< " : "
				<< getStringFromIp(it->second.nextHop)
				<< endl;
	}

	logFile.close();

	while (logFile.is_open());
}

