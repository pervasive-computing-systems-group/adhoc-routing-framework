#ifndef AODV_TEST_H
#define AODV_TEST_H

#include "aodv.h"
#include "print_port.h"
#include <queue>

struct QueuedPacket{
    char* data;
    int length;
    int portId;
    IP_ADDR source;
};

/* AODVTest class
 * For testing AODV with unit tests. */
class AODVTest : public AODV
{
public: 	
	// debugging values 
	static int globalPacketCount;
	static IP_ADDR lastNode; 
	static IP_ADDR lastReceive;

	AODVTest(IP_ADDR ip) : AODV(ip) {}
	AODVTest(const char* ip) : AODV(ip) {}
	~AODVTest() { } 

    int handlePackets() override;

	// Network Monitoring 

	// add/remove node to neighbor list
	void addNeighbor(AODVTest* node);
	void removeNeighbor(AODVTest* node);
    void addPhysicalNeighborOnly(AODVTest* node);
    void removePhysicalNeighborOnly(AODVTest* node);

	// return true if node is neighbor
	bool isNeighbor(AODVTest* node);

	// see if packet was put in queue
	bool packetInRreqBuffer(IP_ADDR dest);

    // Virtual place to store incoming packets
    queue<QueuedPacket> packetQueue;

    RREQHelper* getRREQHelper(){
        return &rreqHelper;
    }

protected:
    int _socketSendPacket(int port, char *buffer, int length, IP_ADDR dest) override;
	void _buildPort(Port*) override;
    void _destroyPort(Port*) override;

private:
	vector<AODVTest*> m_physicalNeighbors;

};

class AODVMonitorTest : public AODVTest 
{
protected:
	int _socketSendPacket(int port, char *buffer, int length, IP_ADDR dest) override;

};



#endif
