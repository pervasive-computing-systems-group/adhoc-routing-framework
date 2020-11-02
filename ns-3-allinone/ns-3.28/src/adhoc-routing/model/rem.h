#ifndef REM_H
#define REM_H

/*********************************
 * rem.h
 *
 * C++ implementation of REgression-based network Monitoring
 * Formerly known as NetworkMonitor.h***
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

#include "network_monitor.h"
#include "rem_model.h"

#include <vector>

class REM : public NetworkMonitor
{
public:
    //  static const int INIT_COUNT = 3; // DEPRECATED. REAL INIT_COUNT IN MODEL
    static const int HOP_COUNT = 2;

    REM() : NetworkMonitor() { clusterHeadIp = getIpFromString(BROADCAST_STR); routing = nullptr; } 
    REM(IP_ADDR nodeIp) : NetworkMonitor(nodeIp) { clusterHeadIp = getIpFromString(BROADCAST_STR); routing = nullptr; } 

    // NS3-TODO:  void initialize(Ptr<Node> parent, Ptr<EnergySource> battery, Ptr<Socket> socket); // initialize node 'parent' with network monitoring service
    // temp:
    void initialize(IP_ADDR parentIp);
    void initializeBatteryModel();
    void initializeRssModel(IP_ADDR pairIp);

    // TODO: Change this to handlePackets
    void handleMonitoringPacketBuffer(char* packet, int length, IP_ADDR source, int port) override;
    void updateLocalModels();

    // get monitoring information 
    double getBatteryLevel(IP_ADDR ownerIp = -1);
    double getRSSBetweenNodes(IP_ADDR pairIp, IP_ADDR ownerIp);

    void updatePairData(const pair_data& pairData) override;

    // update local models with new data points 
    void updateLocalBatteryModel(double batteryLevel);
    void updateLocalRSSModel(IP_ADDR pairIp, double rss);

    // updating local models might result in drastic changes that need to be broadcasted...
    void sendUpdatedModel(PredictionModel* model, IP_ADDR dest);

    // update network models
    void updateNetworkBatteryModel(IP_ADDR ownerIp, const BatteryModel& model);
    void updateNetworkRSSModel(IP_ADDR ownerIp, IP_ADDR pairIp, const RssModel& model);

    // abstract function for getting current battery level
    virtual double getCurrentBatteryLevel() = 0;

    AdhocRoutingProtocol* routing;

    // local models: only one battery model but multiple RSS models
    BatteryModel localBatteryModel;
    // the owner is always this node, the IP_ADDR is the ip address of the paired node 
    map<IP_ADDR,RssModel> localRssModels; 

// NS3-TODO: abstractize?    Ptr<Socket> socket;
protected:
    // get the current time in milliseconds. abstract function for sim and hardware implementations
    virtual uint64_t _getCurrentTimeMS() = 0;
    uint64_t simStartTime;

    IP_ADDR clusterHeadIp;

    // network models: models created by other nodes 
    map<IP_ADDR,BatteryModel> netBatteryModels;
    // int key is the ownerId to a vector of pairIds with associated RssModels
    map<IP_ADDR,map<IP_ADDR,RssModel>> netRssModels;

};

class REMTest : public REM
{
public:
    REMTest() : REM() { simStartTime = _getCurrentTimeMS(); }
    REMTest(IP_ADDR parentIp) : REM(parentIp) { simStartTime = _getCurrentTimeMS(); }

    double getCurrentBatteryLevel() override;

    void drainBattery();

protected:
    // override because we have different time systems depending on hardware, sim, etc
    uint64_t _getCurrentTimeMS() override;

};
#endif
