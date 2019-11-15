#include "adhoc_routing.h"

void AdHocRouting::receivePacketWithPairData(char* packet, int length, IP_ADDR source, pair_data pairData)
{
    this->updatePairData(pairData);
    routing->decodeReceivedPacketBuffer(packet, length, source);
}

void AdHocRouting::sendPacket(char* packet, int length, IP_ADDR finalDestination, IP_ADDR origIP)
{
    routing->sendPacket(packet, length, finalDestination, origIP);
}

void AdHocRouting::updatePairData(pair_data pairData)
{
    if (MONITOR_DEBUG)
        cout << "[DEBUG]: Updating pair data from AdHocRouting" << endl;

    monitor->updatePairData(pairData);

    // after creating new models, update local models in network monitoring 
    monitor->updateLocalModels();
}