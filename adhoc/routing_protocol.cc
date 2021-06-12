#include "routing_protocol.h"

#define ROUTE_DEBUG		0

#include <fstream>

using namespace std;

IP_ADDR getIpFromString(string ipStr)
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

RoutingProtocol::RoutingProtocol(){
	this->m_pAppConnectionHandler = nullptr;
}

RoutingProtocol::RoutingProtocol(IP_ADDR nIP) : ipAddress(nIP) { 
	this->m_pAppConnectionHandler = nullptr;
}

RoutingProtocol::~RoutingProtocol(){
}

bool RoutingProtocol::addSocket(uint32_t nPortNum, AppPacketHandler* pAppPacketHandler) {
	if(!this->m_mSockets.count(nPortNum)) {
		if(ROUTING_DEBUG){
			printf("[ROUTING]:[DEBUG]: Adding socket on port %d\n",nPortNum);
		}
		this->m_mSockets[nPortNum] = this->_protocolCreateSocket(nPortNum, pAppPacketHandler);
		return true;
	}

	return false;
}

bool RoutingProtocol::removeSocket(uint32_t nPortNum){
	if(this->m_mSockets.count(nPortNum)){
		if(ROUTING_DEBUG){
			printf("[ROUTING]:[DEBUG]: Removing socket on port %d\n",nPortNum);
		}
		if(this->_protocolDestroySocket(nPortNum)){
			this->m_mSockets.erase(nPortNum);
			return true;
		}
		return false;
	}
	return false;
}

void RoutingProtocol::addPort(Port* p) {
	if(ports.count(p->getPortId()) == 0) {
		if(ROUTING_DEBUG){
			printf("[ROUTING]:[DEBUG]: Adding port %d\n",p->getPortId());
		}
		ports[p->getPortId()] = p;
		this->_buildPort(p);
	}
}

void RoutingProtocol::removePort(Port* p){
	if(ports.count(p->getPortId())){
		if(ROUTING_DEBUG){
			printf("[ROUTING]:[DEBUG]: Removing port %d\n",p->getPortId());
		}
		this->_destroyPort(p);
		ports.erase(p->getPortId());
	}
}

void RoutingProtocol::setAppConnectionHandler(AppConnectionHandler* pAppConnectionHandler){
	this->m_pAppConnectionHandler = pAppConnectionHandler;

	// Iterate over sockets and set their app connection handlers
	for(auto socket: this->m_mSockets){
		socket.second->setAppConnectionHandler(this->m_pAppConnectionHandler);
	}
}

int RoutingProtocol::sendPacket(Port* p, char* data, int length, IP_ADDR dest, IP_ADDR origIP){
	return sendPacket(p->getPortId(), data, length, dest, origIP);
}

int RoutingProtocol::sendPacket(int portId, char* data, int length, IP_ADDR dest, IP_ADDR origIP) {
	int bytesSent = protocolSendPacket(portId, data, length, dest, origIP);

	if(bytesSent < 0) {
		if(PB_DEBUG) {
			printf("[ROUTING]:[PB_DEBUG]: Buffering packet for %s\n", getStringFromIp(dest).c_str());
		}

		m_oPacketBuffer.storePacket(dest, portId, data, length);
	}

	auto soc = m_mSockets.find(portId);
	if(soc != m_mSockets.end()) {
		soc->second->runAPHSend(bytesSent, data);
	}

	return bytesSent;
}

// Attempts to send one packet from the buffer for each unique destination
int RoutingProtocol::emptyBuffer() {
	int packsSent = 0;

	// Check to see if there are any packets waiting to be sent
	if(m_oPacketBuffer.getNumbPackets()) {
		if(PB_DEBUG) {
			printf("[ROUTING]:[PB_DEBUG]: %li packets waiting\n", m_oPacketBuffer.getNumbPackets());
		}
		// Grab list of destination addresses
		std::deque<int> list;
		m_oPacketBuffer.getReceiverList(&list);

		// Pop-off one packet for each destination and attempt to send it
		for(std::deque<int>::iterator it = list.begin(); it != list.end(); it++) {
			BufferedPacket bufferedPacket;
			m_oPacketBuffer.getPacket(*it, &bufferedPacket);
			int bytesSent = protocolSendPacket(bufferedPacket.getPortId(), bufferedPacket.getBuffer(), bufferedPacket.getLength(), bufferedPacket.getDestination());

			// TODO: This will put the packet at the back, we should change this so that it isn't rotating packets through the internal buffer queue
			if(bytesSent < 0) {
				m_oPacketBuffer.storePacket(bufferedPacket.getDestination(), bufferedPacket.getPortId(), bufferedPacket.getBuffer(), bufferedPacket.getLength());
			}
			else {
				if(PB_DEBUG) {
					printf("[ROUTING]:[PB_DEBUG]: Found route to %s\n", getStringFromIp(bufferedPacket.getDestination()).c_str());
				}

				// Run app packet handler only if we sent the packet
				auto soc = m_mSockets.find(bufferedPacket.getPortId());
				if(soc != m_mSockets.end()) {
					soc->second->runAPHSend(bytesSent, bufferedPacket.getBuffer());
				}
				packsSent++;
			}
		}
	}

	return packsSent;
}

// Handles the receiving or processing of all packets when implementing this should query each of the
// sockets corresponding to each port and then "give" the data to each port
int RoutingProtocol::handlePackets() {
	Message message;
	int count = 0;

	for(pair<const uint32_t, Socket*>& socPair : m_mSockets) {
		while(socPair.second->getMessage(message)) {
			protocolHandlePacket(socPair.second, &message);
			socPair.second->runAPHReceive(&message);
			count++;
		}
	}

	return count;
}

bool RoutingProtocol::linkExists(IP_ADDR dest) {
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

void RoutingProtocol::resetLinks()
{
	globalMux.lock();
	m_neighbors.clear();
	globalMux.unlock();
}

void RoutingProtocol::addLink(IP_ADDR node)
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

bool RoutingProtocol::_isAppConnected(IP_ADDR ip){
	if(this->m_pAppConnectionHandler != nullptr){
		return this->m_pAppConnectionHandler->isConnected(ip);
	}
	return true;
}
