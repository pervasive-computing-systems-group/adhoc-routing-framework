#include "routing_protocol.h"

#define ROUTE_DEBUG		0

#include <fstream>

using namespace std;

IP_ADDR getIpFromString(const string& ipStr)
{
	IP_ADDR ip;

	struct sockaddr_in sa;
	inet_pton(AF_INET, ipStr.c_str(), &(sa.sin_addr));

	ip = sa.sin_addr.s_addr;

	return ip;
}

string getStringFromIp(IP_ADDR ip)
{
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ip), str, INET_ADDRSTRLEN);

	string ipString(str);

	return ipString;
}

RoutingTable::RoutingTable()
{
	if (ROUTE_DEBUG)	
		cout << "[TABLE]:[DEBUG]: Routing table created" << endl;
}

RoutingTable::~RoutingTable()
{
	this->table.clear();
}


IP_ADDR RoutingTable::getNextHop(const IP_ADDR dest)
{
	if (TABLE_DEBUG)
		cout << "[TABLE]:[DEBUG]: Getting next hop for " << getStringFromIp(dest) << endl;

	IP_ADDR nextHop;

	if (this->table.count(dest)) // Entry exists
	{
		
		nextHop = table[dest].nextHop;
	}
	else // Table entry does not exist
	{
		nextHop = 0;
	}

	return nextHop;
}

void RoutingTable::updateTableEntry(const IP_ADDR dest, const IP_ADDR nextHop)
{
	// check if this entry exists 
	if (this->table.count(dest))
	{
		// entry exists, update existing 
		this->table[dest].nextHop = nextHop;
		this->table[dest].ttl = DEFAULT_TTL;
	}
	else
	{
		// no entry, create new 
		if(TABLE_DEBUG){
			cout << "[TABLE]: [DEBUG]: Creating new entry" << endl;
		}
		TableInfo info;
		info.dest = dest;
		info.nextHop = nextHop;
		info.ttl = DEFAULT_TTL;
	
		this->table[dest] = info;
	}
}

AdhocRoutingProtocol::AdhocRoutingProtocol(){
}
AdhocRoutingProtocol::~AdhocRoutingProtocol(){
}

void AdhocRoutingProtocol::addPort(Port* p){
	if(m_ports.count(p->getPortId()) == 0){
		if(ROUTING_DEBUG){
			printf("[ROUTING]:[DEBUG]: Adding port %d\n",p->getPortId());
		}
		m_ports[p->getPortId()] = p;
		this->_buildPort(p);
	}
}

void AdhocRoutingProtocol::removePort(Port* p){
	if(m_ports.count(p->getPortId())){
		if(ROUTING_DEBUG){
			printf("[ROUTING]:[DEBUG]: Removing port %d\n",p->getPortId());
		}
		this->_destroyPort(p);
		m_ports.erase(p->getPortId());
	}
}

bool AdhocRoutingProtocol::sendPacket(Port* p, char* data, int length, IP_ADDR dest, IP_ADDR origIP){
	return sendPacket(p->getPortId(), data, length, dest, origIP);
}

bool AdhocRoutingProtocol::linkExists(IP_ADDR dest) {
    if (MONITOR_DEBUG){
        cout << "[ROUTING]:[DEBUG]: Checking if link exists from "
             << getStringFromIp(getIp()) << " to " << getStringFromIp(dest)
             << endl;
	}

	// if this is a broadcast, the link always exists.
	if (getStringFromIp(dest) == BROADCAST_STR)
	{
		if(ROUTING_DEBUG)
			cout << "[ROUTING]:[DEBUG]: Broadcast link always exists" << endl;
		return true;
	}	

	// Lock neighbor mux because we are checking neighbors
	neighborMux.lock();

    for (IP_ADDR ip : m_neighbors) {
		if(ROUTING_DEBUG)
			cout << "[ROUTING]:[DEBUG]: Checking neighbor link: " << getStringFromIp(ip) << endl;
        if (dest == ip) {
            if (MONITOR_DEBUG)
                cout << "[ROUTING]:[DEBUG]: Link exists!" << endl;

			neighborMux.unlock();
            return true;
        }
    }
	neighborMux.unlock();

    if (MONITOR_DEBUG)
        cout << "[ROUTING]:[DEBUG]: Link does not exist." << endl;

    return false;
}

void AdhocRoutingProtocol::resetLinks()
{
	globalMux.lock();
	m_neighbors.clear();
	globalMux.unlock();
}

void AdhocRoutingProtocol::addLink(IP_ADDR node)
{
	if (ROUTING_DEBUG)
		cout << "[ROUTING]:[DEBUG]: Adding link on " << getStringFromIp(getIp()) << " to node " << getStringFromIp(node) << endl;

	bool exists = false;
	for (IP_ADDR link : m_neighbors)
	{
		if (node == link)
			exists = true;
	}

	if (!exists)
	{
		m_neighbors.push_back(node);
		// TODO: ADD AS ONE HOP NEIGHBOR TO ROUTING TABLE
		m_pRoutingTable->updateTableEntry(node, node);
		if (ROUTING_DEBUG)
			cout << "[ROUTING]:[INFO]: One hop neighbor: " << getStringFromIp(node) << " added to routing table." << endl; 
	}
}


const vector<IP_ADDR>& AdhocRoutingProtocol::getCopyOfNeighbors()
{
	return m_neighbors;
}
