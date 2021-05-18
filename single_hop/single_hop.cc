#include "single_hop.h"
#include "string.h"
#include <chrono>
#include <fstream>
#include <stdio.h>
#include <thread>

/******************************
 * Constructors + Destructors
 ******************************/

SingleHop::SingleHop() { }

SingleHop::SingleHop(const char* ip) : SingleHop(getIpFromString(ip)){}

SingleHop::SingleHop(IP_ADDR ip) {
    if(SINGLE_HOP_DEBUG)
        cout << "[SH]:[DEBUG]: Created new single-hop routing protocol." << endl;

    this->ipAddress = ip;
}

SingleHop::~SingleHop() { }

/******************************
 * Public Functions
 ******************************/
bool SingleHop::sendPacket(int portId, char* packet, int length, IP_ADDR dest, IP_ADDR origIP) {
/*
	// by default the next hop is the final destination in case this is a broadcast
    IP_ADDR nextHop = dest;

    if (getStringFromIp(dest) != BROADCAST_STR)
    {
        if (AODV_DEBUG)
            cout << "[AODV]:[INFO]: Next hop: " << getStringFromIp(this->getTable()->getNextHop(origIP)) << endl;

        // by default this node is the originator
        if (-1 == signed(origIP))
            origIP = getIp();

        if (length < 0)
            cerr << "[AODV]:[ERROR]: Negative packet length." << endl;

        // if entry exists in routing table, send it!
        // check routing table
        if(this->m_pRoutingTable == nullptr){
            fprintf(stderr, "[AODV]:[ERROR]: The routing table is NULL\n");
            exit(255);
        }else{
            if (AODV_DEBUG)
                cout << "[AODV]:[DEBUG]: Getting next hop for " << getStringFromIp(dest) << endl;
            nextHop = m_pRoutingTable->getNextHop(dest);
        }

        if (false == getTable()->getIsRouteActive(dest)) {
            BufferedPacket bufferedPacket(portId, packet, length);

            // Put this packet in a buffer to be sent when a route opens up
            if (this->rreqPacketBuffer.count(dest)) {
                this->rreqPacketBuffer[dest]->push(bufferedPacket);
            } else {
                SafeCircularQueue<BufferedPacket>* q = new SafeCircularQueue<BufferedPacket>(PACKET_BUFFER_SIZE);
                q->push(bufferedPacket);
                this->rreqPacketBuffer[dest] = q;
            }

            if (nextHop != 0) {
                // this route is temporarily unavailable... check if actively
                // finding a route
                // TODO: send a RERR

                //            return;
            }

            if (AODV_DEBUG)
                cout << "[AODV]:[DEBUG]: No existing route, creating RREQ message." << endl;

            rreqPacket rreq = this->rreqHelper.createRREQ(dest);

            // start a thread for THIS rreq and wait for a response
            if (RREQ_RETRIES) {
                thread waitForResponse(retryRouteRequestIfNoRREP, this, rreq,
                                    RREQ_RETRIES);
                waitForResponse.detach();
            }
            _broadcastRREQBuffer(rreq);

            return true;
        }
        else if (AODV_DEBUG)
        {
            cout << "[AODV]:[DEBUG]: Route is active, no RREQ necessary." << endl;
        }
    }

    if (AODV_DEBUG)
        cout << "[AODV]:[DEBUG]: Route exists. Routing from "
             << getStringFromIp(getIp()) << " to "
             << getStringFromIp(dest) << endl;

    // add aodv header to buffer
    char *buffer = (char *)(malloc(HEADER_SIZE + length));
    uint8_t zero = 0x00;
    memcpy(buffer, &(zero), 1);
    buffer++;
    memcpy(buffer, &dest, 4);
    buffer += 4;
    memcpy(buffer, &origIP, 4);
    buffer += 4;

    // copy data into packet
    memcpy(buffer, packet, length);
    // reset packet
    buffer -= HEADER_SIZE;

    if (linkExists(nextHop)) {
        _socketSendPacket(portId, buffer, length + HEADER_SIZE, nextHop);
    } else {
        repairLink(portId, nextHop, dest, buffer, length, origIP);
    }

    free(buffer);
*/
    return true;
}


/******************************
 * Protected Functions
 ******************************/

void SingleHop::_handlePacket(int port, char *packet, int length, IP_ADDR source) {
/*
	if(port == ROUTING_PORT){
        _handleAODVPacket(packet, length, source);
    }else{
        // get final destination
        IP_ADDR finalDestination;
        memcpy(&finalDestination, &(packet[1]), 4);
        IP_ADDR origIP;
        memcpy(&origIP, &(packet[5]), 4);
        if (this->getIp() == finalDestination || finalDestination == getIpFromString(BROADCAST_STR)) {
            // We are the final destinatin
            // Have socket handle the packet
            packet += HEADER_SIZE; // Get only the data part of the packet
            if(ports.count(port)){
                if(AODV_DEBUG)
                    std::cout << "[AODV]:[INFO]: Received packet on port " << port << endl; 
                ports[port]->handlePacket(packet, length - HEADER_SIZE, source);
            }else{
                fprintf(stderr, "[AODV]:[ERROR]: Received packet on port with no port handler\n");
            }
            packet -= HEADER_SIZE;
        }else{
            // send the packet to final destination - will check routing table
            // strip header and send packet
            packet += HEADER_SIZE;
            sendPacket(port, packet, length - HEADER_SIZE, finalDestination, origIP);
            packet -= HEADER_SIZE;
        }
    }
    */
}

void SingleHop::_handlePacket(Port* p, char *buffer, int length, IP_ADDR source){
//    _handlePacket(p->getPortId(), buffer, length, source);
}


bool SingleHop::_socketSendPacket(Port* p, char *buffer, int length, IP_ADDR dest){
    return _socketSendPacket(p->getPortId(), buffer, length, dest);
}

/******************************
 * Helper Functions
 ******************************/
