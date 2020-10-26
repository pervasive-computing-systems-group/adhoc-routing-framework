/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "adhoc-routing-helper.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4.h"
#include "ns3/simulator.h"
#include "ns3/inet-socket-address.h"
#include "ns3/tag.h"
#include "ns3/udp-header.h"

#include "ns3/network_monitor.h"
#include "ns3/aodv_sim.h"
#include "ns3/rem_sim.h"
#include "ns3/port.h"

#include <iostream>

namespace ns3 {

std::map<IP_ADDR, Ptr<Node>> AdHocRoutingHelper::m_existingNodes;

int AdHocRoutingHelper::AdHocSendPacket(char* buffer, int length, IP_ADDR dest, int port, IP_ADDR source)
{
    if (DEBUG) 
        std::cout << "[ADHOC_HELPER]:[DEBUG]: Source node = " << getStringFromIp(source) 
                  << " to destination node = " << getStringFromIp(dest) << std::endl;

    Ptr<Node> sourceNode = AdHocRoutingHelper::m_existingNodes[source];
    Ptr<Node> destNode = AdHocRoutingHelper::m_existingNodes[dest];

    if (dest == getIpFromString(BROADCAST_STR))
    {
        // set destination to broadcast
        Ipv4Address destAddr(getIpFromString(BROADCAST_STR));

        // create socket between nodes 
        Ptr<Socket> socket = Socket::CreateSocket(sourceNode, UdpSocketFactory::GetTypeId());
        InetSocketAddress remote = InetSocketAddress(destAddr, port);
        // allow broadcast 
        socket->SetAllowBroadcast(true);
        // connect the nodes with the new socket  
        socket->Connect(remote); 

        // create packet 
        Ptr<Packet> packet = Create<Packet>(reinterpret_cast<const uint8_t*> (buffer), length); 
        Ipv4Header header;
        // fill header
        header.SetSource(sourceNode->GetObject<Ipv4>()->GetAddress(1,0).GetLocal());
        packet->AddHeader(header);

        return socket->Send(packet);
    }
    else 
    {
        Ptr<Ipv4> destIpv4 = destNode->GetObject<Ipv4>(); // Get Ipv4 instance of the node
        Ipv4Address destAddr = destIpv4->GetAddress (1, 0).GetLocal();  

        // create socket 
        Ptr<Socket> socket = Socket::CreateSocket(sourceNode, UdpSocketFactory::GetTypeId());
        InetSocketAddress remote = InetSocketAddress(destAddr, port);
        socket->SetAllowBroadcast(false);
        socket->Connect(remote);  

        // create packet 
        Ptr<Packet> packet = Create<Packet>(reinterpret_cast<const uint8_t*> (buffer), length); 
        Ipv4Header header;
        // fill header
        header.SetSource(sourceNode->GetObject<Ipv4>()->GetAddress(1,0).GetLocal());
        packet->AddHeader(header);

        return socket->Send(packet);
    }
}

void AdHocRoutingHelper::receivePacket(Ptr<Socket> socket)
{
    Ptr<Packet> packet;
    int packetSize = 0;

    // Add this packet to the buffer of current packets and make callback to remove it in 1 second 
    while (packet = socket->Recv())
    {
        packetSize = packet->GetSize();
        Ipv4Header header;
        packet->RemoveHeader(header); 

        // get rss data from packet 
        WifiTag tag;
        packet->PeekPacketTag(tag);

        uint32_t length = packet->GetSize();
        uint8_t* packetBuffer = (uint8_t*)(malloc(length));

        packet->CopyData(packetBuffer, length);
        
        Ptr<Ipv4> ipv4 =  socket->GetNode()->GetObject<Ipv4> (); // Get Ipv4 instance of the node

        if (DEBUG)
            std::cout << "[DEBUG]: Received data on port " << socket->m_port << std::endl;

        // add aodv object
        Ipv4Address addr = header.GetSource(); 
        uint8_t* ipBuf = (uint8_t*)(malloc(4)); 
        addr.Serialize(ipBuf);

        IP_ADDR source;
        memcpy(&(source),(ipBuf),4);

        if (DEBUG)
            std::cout << "[DEBUG]: Node " << getStringFromIp(socket->GetNode()->m_nodeIp) << " from Node " << getStringFromIp(source) << std::endl;

        pair_data packetPairData;
        packetPairData.pairIp = source; 
        packetPairData.rss = tag.GetRssValue();

        if (DEBUG)
            std::cout << "[DEBUG]: Received RSS: " << packetPairData.rss << std::endl;

        // Add the packet to this nodes buffer 
        SimPacket packet;
        packet.data = (char*)packetBuffer;
        packet.length = length;
        packet.packetPairData = packetPairData;
        packet.portId = socket->m_port;
        packet.source = source;

        AdHocRoutingHelper* adhocRoutingHelper = socket->GetNode()->m_AdHocRoutingHelper;

        // Log this interaction as a 'hello' message
        adhocRoutingHelper->helloMonitor->receiveHelloMessage(source);

        // Update network statistics
        // Update rss 
        adhocRoutingHelper->updateLinkRss(source, packetPairData.rss);
        // Update bandwidth 
        adhocRoutingHelper->updateLinkBandwidth(source, packetSize);

        // Handle packet
        adhocRoutingHelper->handleSimPackets(packet);

        free(packetBuffer);
        free(ipBuf);
    }
}


int AdHocRoutingHelper::getLinkBandwidthBits(AdHocRoutingHelper* targetHelper)
{
	int bandwidthByLink = 0;
	Ptr<Node> targetNode = targetHelper->getNode();
	Ptr<Object>  targetObject = targetNode;
	Ptr<MobilityModel> model = targetObject->GetObject<MobilityModel>();

	Ptr<Object> thisObject = this->m_node;
	double distActual = model->GetDistanceFrom(thisObject->GetObject<MobilityModel>());


	if (DEBUG)
	{
		std::cout << "[DEBUG] [BANDWIDTH]: Distance between " << this->getIpAddressStr()
			<< " and " << getStringFromIp(targetNode->m_nodeIp)
			<< " is: " << distActual << std::endl;
	}

	if(this->linkExists(targetHelper->getIp()))
	{
		// Determine bandwidth, the minimum as a portion of available bandwith OR quadratic loss by distance
		bandwidthByLink = (int)fmin(((double)m_availableBandwidthBits * this->getBandwidthShare(targetNode->m_nodeIp)),
				(BANDWIDTH_0 * sqrt(BANDWIDTH_DIST_0 / distActual)));

		if (DEBUG)
		{
			std::cout << "[DEBUG] [BANDWIDTH]: bandwidth portion: "
					<< ((double)m_availableBandwidthBits * this->getBandwidthShare(targetNode->m_nodeIp))
					<< ",  bandwidth by distance: " << (BANDWIDTH_0 * sqrt(BANDWIDTH_DIST_0 / distActual))
					<< ", reporting: " << bandwidthByLink << std::endl;
		}
	}

	return bandwidthByLink;
}

// Returns the number of bits sent to destIP over the last second
int AdHocRoutingHelper::getLinkBitsSent(IP_ADDR destIP)
{
	int retVal = 0;

	// If this node is in the map, get the number of bits
	std::map<IP_ADDR, int>::iterator it = m_bandwidthUsedMap.find(destIP);
	if(it != m_bandwidthUsedMap.end())
	{
		retVal = it->second;
	}

	return retVal;
}

void AdHocRoutingHelper::increaseAvailableBandwidthByBits(IP_ADDR linkIP, int numberOfBits)
{
	// Remove packet size to bandwidth usage map
	std::map<IP_ADDR, int>::iterator it = m_bandwidthUsedMap.find(linkIP);
	if(it != m_bandwidthUsedMap.end())
	{
		if(it->second >= numberOfBits)
		{
			it->second = it->second - numberOfBits;
		}
		else
		{
			// something went wrong... just clear map
	        fprintf(stderr, "[ADHOC_HELPER]:[ERROR]: Removing unaccounted for bandwidth\n");
			it->second = 0;
		}
	}
	else
	{
		// something went wrong... add IP to map
        fprintf(stderr, "[ADHOC_HELPER]:[ERROR]: Tried to remove bandwidth used by none-familiar IP address\n");
		m_bandwidthUsedMap.insert(std::pair<int, int>(linkIP, 0));
	}
	this->m_availableBandwidthBits += numberOfBits;
}

void RemovePacketFromBandwidthMetric(AdHocRoutingHelper* adhocRoutingHelper, IP_ADDR linkIP, int numberOfBits)
{
    adhocRoutingHelper->increaseAvailableBandwidthByBits(linkIP, numberOfBits);
}

void AdHocRoutingHelper::updateLinkBandwidth(IP_ADDR linkIP, uint32_t bandwidthBytes)
{
    // Add this packet to current bandwidth and remove it after a second 
    int numberOfBits = bandwidthBytes * 8;
    this->m_availableBandwidthBits -= numberOfBits; 

    if (signed(this->m_availableBandwidthBits) < 0)
    {
        int difference = abs(signed(this->m_availableBandwidthBits));
        this->m_availableBandwidthBits += difference;
        numberOfBits -= difference;
    }

    // Add packet size to bandwidth usage map
    std::map<IP_ADDR, int>::iterator it = m_bandwidthUsedMap.find(linkIP);
    if(it != m_bandwidthUsedMap.end())
    {
        it->second = it->second + numberOfBits;
    }
    else
    {
        m_bandwidthUsedMap.insert(std::pair<int, int>(linkIP, numberOfBits));
    }

    // since our metric is in Mbps, we will remove these bits in 1 second 
    Simulator::Schedule(Seconds(1.0), &RemovePacketFromBandwidthMetric, this, linkIP, numberOfBits);
}

uint64_t AdHocRoutingHelper::getNs3SimulatedTimeMS()
{
    return (uint32_t)Simulator::Now().GetMilliSeconds();
}

double AdHocRoutingHelper::getNs3SimulatedBattery(IP_ADDR nodeIp)
{
    // get the battery level of node ip
    Ptr<Node> node = m_existingNodes[nodeIp];
    return node->m_battery;
}

void AdHocRoutingHelper::waitSimulatedTimeForHelloMonitor(int DURATION_MS, SimHelloMonitor* waitingHello)
{
    // duration of 1 so we do 1 iteration and then stop
    // we do this because in a discrete event simulator we can't keep track of time easily
    // instead we schedule sends every DURATION_MS
    waitingHello->sendHellos(1);

    Simulator::Schedule(MilliSeconds(DURATION_MS), &waitSimulatedTimeForHelloMonitor, DURATION_MS, waitingHello);
}

Ptr<Node> AdHocRoutingHelper::getNode()
{
	return m_node;
}

void dumbySleep(int DURATION_MS)
{
}

AdHocRoutingHelper::AdHocRoutingHelper(Ptr<Node> node, IP_ADDR nodeIp) : SimAODV(nodeIp)
{
    m_node = node;
    m_node->m_AdHocRoutingHelper = this;

    AdHocRoutingHelper::m_existingNodes[nodeIp] = node;
    // Override simSocketSendPacket to use ns3 sockets
    this->simSocketSendPacket = &AdHocSendPacket;
    std::cout << "[ADHOC_HELPER]:[INFO]: Using ns3 simSocketSendPacket" << std::endl;

    // create network monitoring
    // NOTE: This is for battery level ONLY right now 
    REMSim* rem = new REMSim(nodeIp);
    rem->getSimulatedBatteryLevel = &(AdHocRoutingHelper::getNs3SimulatedBattery);
    rem->getSimulatedTime = &(AdHocRoutingHelper::getNs3SimulatedTimeMS);
    rem->routing = this;

    rem->initialize(nodeIp);
    // assign SimAODV networkMonitor to this rem
    this->networkMonitor = rem;
    rem->setPortId(MONITOR_PORT);

    // add port to AdHocRoutingHelper
    std::cout << "[ADHOC_HELPER]:[INFO]: Added REM port" << std::endl;
    this->addPort(rem);

    // create hello monitoring 
    SimHelloMonitor* hello = new SimHelloMonitor(HELLO_PORT, this);
    this->helloMonitor = hello;
    // because this is a discrete event simulator, actually waiting doesn't make sense
    // we need to schedule
    hello->waitSimulatedTime = &(dumbySleep);
    hello->getSimulatedTime = &(AdHocRoutingHelper::getNs3SimulatedTimeMS);

    // add port to AdHocRoutingHelper
    std::cout << "[ADHOC_HELPER]:[INFO]: Added hello monitor port" << std::endl;
    this->addPort(hello);

    // create print packet port 
    PrintPort* printPort = new PrintPort(DATA_PORT);
    this->addPort(printPort);
    std::cout << "[ADHOC_HELPER]:[INFO]: Added print port" << std::endl;

    std::cout << "[WARNING]: Must start a recurring event to call monitor->updateLocalModels" << std::endl;
    std::cout << "[WARNING]: Use AdHocRoutingHelper sendPacket and receivePacket functions." << std::endl;
}

// close ns3 namespace
}
