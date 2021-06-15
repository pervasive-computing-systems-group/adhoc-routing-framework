#include "aodv_routing_table.h"

AODVRoutingTable::AODVRoutingTable()
{
	if (TABLE_DEBUG)
		cout << "[TABLE]:[DEBUG]: Created new aodv routing table." << endl;
}

AODVRoutingTable::~AODVRoutingTable()
{

}

IP_ADDR AODVRoutingTable::getNextHop(const IP_ADDR dest)
{
	if (TABLE_DEBUG)
		cout << "[AODVTABLE]:[DEBUG]: Getting next hop for " << getStringFromIp(dest) << endl;

	IP_ADDR nextHop;
	if (m_aodvTable.count(dest))
	{
		nextHop = m_aodvTable[dest].nextHop;
	}
	else
	{
		nextHop = 0;
	}

	return nextHop;
}

uint32_t AODVRoutingTable::getDestSequenceNumber(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, return dest sequence number  
		return ((AODVInfo*)&(this->m_aodvTable[dest]))->destSequenceNumber; 
	}
	else
	{
		return 0;
	}
}

uint8_t AODVRoutingTable::getDestHopCount(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, return dest hop count  
		return ((AODVInfo*)&(this->m_aodvTable[dest]))->hopCount; 
	}
	else
	{
		return 0;
	}
}

uint32_t AODVRoutingTable::getLastRREQId(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, return dest sequence number  
		return ((AODVInfo*)&(this->m_aodvTable[dest]))->lastRREQId; 
	}
	else
	{
		return 0;
	}
}

void AODVRoutingTable::setDestSequenceNumber(const IP_ADDR dest, uint32_t destSeqNum)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, update dest sequence number  
		((AODVInfo*)&(this->m_aodvTable[dest]))->destSequenceNumber = destSeqNum; 
	}
	else
	{
		cout << "[TABLE]:[ERROR]: Error update sequence number: Unknown table entry." << endl;
		return;
	}
}

void AODVRoutingTable::setHopCount(const IP_ADDR dest, uint8_t hopCount)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, update dest sequence number  
		((AODVInfo*)&(this->m_aodvTable[dest]))->hopCount = hopCount; 
	}
	else
	{
		cout << "[TABLE]:[ERROR]: Error update hop count: Unknown table entry." << endl;
		return;
	}
}

void AODVRoutingTable::setLastRREQId(const IP_ADDR dest, uint32_t lastRREQId)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, update dest sequence number  
		((AODVInfo*)&(this->m_aodvTable[dest]))->lastRREQId = lastRREQId; 
	}
	else
	{
		cout << "[TABLE]:[ERROR]: Error update last rreq id: Unknown table entry." << endl;
		return;
	}
}

void AODVRoutingTable::setIsRouteActive(const IP_ADDR dest, bool active)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{
		// entry exists, update dest sequence number  
		if(TABLE_DEBUG){
			cout << "[TABLE]:[DEBUG]: Setting " << getStringFromIp(dest) << " to INACTIVE " << active << endl;
		}
		this->m_aodvTable[dest].active = active;
	}
	else
	{
		cout << "[TABLE]:[ERROR]: Error update active: Unknown table entry." << endl;
		return;
	}
}

void AODVRoutingTable::updateAODVRoutingTableFromRREQ(rreqPacket* receivedRREQ, IP_ADDR sourceIP)
{
	if (TABLE_DEBUG)
		cout << "[TABLE]:[DEBUG]: Updating routing table from RREQ packet" << endl;

	if (receivedRREQ->hopCount == 0)
		cerr << "[TABLE]:[ERROR]: HOP COUNT IS ZERO. FROM " << getStringFromIp(sourceIP) << endl;

	if (  receivedRREQ->origSeqNum > getDestSequenceNumber(receivedRREQ->origIP)
	   || getCostOfDest(receivedRREQ->origIP) > getCostOfRREQ(*receivedRREQ))
	{
		this->updateTableEntry(receivedRREQ->origIP, sourceIP);

		this->setDestSequenceNumber(receivedRREQ->origIP, receivedRREQ->origSeqNum);
		this->setHopCount(receivedRREQ->origIP, receivedRREQ->hopCount);
		this->setLastRREQId(receivedRREQ->origIP, receivedRREQ->rreqID);
	}
}

void AODVRoutingTable::updateAODVRoutingTableFromRREP(rrepPacket* receivedRREP, IP_ADDR sourceIP)
{
	if (TABLE_DEBUG)
		cout << "[TABLE]:[DEBUG]: Updating routing table from RREP packet" << endl;

	if (  receivedRREP->destSeqNum > getDestSequenceNumber(receivedRREP->destIP)
	   || getCostOfDest(receivedRREP->destIP) > getCostOfRREP(*receivedRREP)
	   || false == getIsRouteActive(receivedRREP->destIP))
	{
		this->updateTableEntry(receivedRREP->destIP, sourceIP);
		this->setDestSequenceNumber(receivedRREP->destIP, receivedRREP->destSeqNum);
		this->setHopCount(receivedRREP->destIP, receivedRREP->hopCount);
	}
	else 
		cout << "[TABLE]:[WARNING]: Routing table NOT updated from RREP packet." << endl;
}

void AODVRoutingTable::updateTableEntry(const IP_ADDR dest, const IP_ADDR nextHop)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{	
		// entry exists, update existing 
		if (TABLE_DEBUG)
			cout << "[TABLE]:[DEBUG]: Updating existing AODV entry" 
				 << " dest: " << getStringFromIp(dest) << " next hop: " 
				 << getStringFromIp(nextHop) << endl;
		this->m_aodvTable[dest].nextHop = nextHop;
		this->m_aodvTable[dest].ttl = DEFAULT_TTL;
	}
	else
	{
		// no entry, create new 
		if (TABLE_DEBUG)
			cout << "[TABLE]:[DEBUG]: Creating new AODV entry" << endl;
		AODVInfo info;
		info.dest = dest;
		info.nextHop = nextHop;
		info.ttl = DEFAULT_TTL;
	
		this->m_aodvTable[dest] = info;
	}

	this->m_aodvTable[dest].active = true;
}

void AODVRoutingTable::removeTableEntry(const IP_ADDR dest)
{
	// check if this entry exists 
	if (this->m_aodvTable.count(dest))
	{	
		// entry exists, delete entry 
		if (TABLE_DEBUG)
			cout << "[TABLE]:[DEBUG]: Setting table entry as inactive" << endl;

		this->m_aodvTable[dest].active = false;
	}
	else
	{
		// no entry, create new 
		if (TABLE_DEBUG)
			cout << "[TABLE]:[DEBUG]: Error. Tried to erase non-existent table entry." << endl;
	}
}

int AODVRoutingTable::getCostOfDest(const IP_ADDR dest)
{	
	if(AODV_WARNING)
		printf("[TABLE]:[WARNING]: getCostOfDest is currently using hopcount\n");
	// currently using hopcount...
	return getDestHopCount(dest);
}

int AODVRoutingTable::getCostOfRREQ(const rreqPacket rreq)
{	
	if(AODV_WARNING)
		printf("[TABLE]:[WARNING]: getCostOfRREQ is currently using hopcount\n");
	// currently using hopcount...
	return rreq.hopCount;
}

int AODVRoutingTable::getCostOfRREP(const rrepPacket rrep)
{
	if(AODV_WARNING)
		printf("[TABLE]:[WARNING]: getCostOfRREP is currently using hopcount\n");
	// currently using hopcount...
	return rrep.hopCount;
}

bool AODVRoutingTable::getIsRouteActive(const IP_ADDR dest)
{	
	if (m_aodvTable.count(dest) > 0)
		return m_aodvTable[dest].active;
	else 
		return false;
}
