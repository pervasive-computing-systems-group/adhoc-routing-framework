// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>
#include "hardware/hardware_aodv.h"
#include "aodv/test.h"

using namespace std;

int main(){
    RoutingProtocol* haodv = new HardwareAODV(MY_IP_ADDR);
    PrintPort* printPort = new PrintPort(DATA_PORT);
    haodv->addPort(printPort);

    char *msg = (char *)(malloc(16));
    string message = "Hello World!";
    memcpy(msg, &message, 16);

    vector<string> ips = { 
        "192.168.1.2",
        "192.168.1.3"
    };

    while(true){
        // Send packets to all ips
        for(auto ip : ips){
            uint32_t dest = getIpFromString(ip);
            if(haodv->sendPacket(printPort->getPortId(), msg, message.length(), dest) == -1){
                printf("[DO ADHOC]:[DEBUG]: unable to currently send data\n");
            }
            printf("[DO ADHOC]:[DEBUG]: Sent ");
            printPacket(stdout, msg, message.length());
            printf(" to %s\n", ip.c_str());
        }
        // Handle packets
        int handleCount = haodv->handlePackets();
        if(handleCount > 0){
            printf("[DO ADHOC]:[DEBUG]:Handled %d AODV packets\n", handleCount);
        }
    }

    // Clean memory up
    delete printPort;
    delete haodv;
}
