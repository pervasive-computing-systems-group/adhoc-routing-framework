// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>

#include "hardware_led_aodv.h"
#include "aodv_test.h"
#include "hardware_sh_ap.h"
#include "hardware_sh_station.h"
#include "LED_APH_SHData.h"

using namespace std;

int main(){
	/// Networking Settings
	string message = "Hello World!";
	char* msg = strdup(message.c_str());
	vector<string> ips = { "192.168.1.1" };
	RoutingProtocol* routingPrtcl;


	if(RT_PROTOCOL == USE_SINGLE_HOP) {
		printf("[TEST ADHOC]: Using SINGLE-HOP, ");
		/// Setup routing protocol
		// TODO: Create data port packet handler

		if(SH_NODE_TYPE == AP_NODE) {
			printf("as access point!\n");
			// Create routing protocol using LED_APH_SHData app
			routingPrtcl = new HardwareSHAP(MY_IP_ADDR, DATA_PORT, nullptr);

			/// Main loop to read/send packets
			while(true) {
				// Read packets, access point doesn't send anything
				routingPrtcl->handlePackets();
			}
		}
		else if(SH_NODE_TYPE == STATION_NODE) {
			printf("as station!\n");
			// TODO: Create data port packet handler
			routingPrtcl = new HardwareSHStation(MY_IP_ADDR, DATA_PORT, nullptr);

			/// main loop to read/send packets
			std::chrono::milliseconds last_send_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
			while(true) {
				// Send a packet every second
				std::chrono::milliseconds current_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

				if((current_time - last_send_time).count() > 1000) {
					last_send_time = current_time;
					for(auto ip : ips) {
						uint32_t dest = getIpFromString(ip);
						if(routingPrtcl->sendPacket(DATA_PORT, msg, message.length() + 1, dest) == -1){
							fprintf(stderr, "[TEST ADHOC]:[ERROR]: Unable to send packet\n");
						}
					}
				}

				routingPrtcl->handlePackets();
			}
		}
	}
	else if(RT_PROTOCOL == USE_AODV) {
		/// Setup
		printf("[TEST ADHOC]: Using AODV\n");
		routingPrtcl = new HardwareLedAODV(MY_IP_ADDR);
        // TODO: Create data loging port
		Port* printPort = new PrintPort(DATA_PORT);
		routingPrtcl->addPort(printPort);

		// Network
		std::chrono::milliseconds last_send_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		while(true) {
			// Send packets to all ips every secondPort* printPort = new PrintPort(DATA_PORT);
			std::chrono::milliseconds current_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

			if((current_time - last_send_time).count() > 1000){
				last_send_time = current_time;
				for(auto ip : ips){
					uint32_t dest = getIpFromString(ip);
					if(routingPrtcl->sendPacket(printPort->getPortId(), msg, message.length()+1, dest) == -1){
						printf("[TEST ADHOC]:[DEBUG]: Unable to send packet (not connected or an error)\n");
					}else{
                        printf("[TEST ADHOC]:[DEBUG]: Sent ");
                        printPacket(stdout, msg, message.length()+1);
                        printf(" to %s\n", ip.c_str());
                    }
				}
			}

			// Handle packets
			int handleCount = routingPrtcl->handlePackets();
			if(handleCount > 0){
				printf("[TEST ADHOC]:[DEBUG]: Handled %d packets\n", handleCount);
			}
		}

		// Clean memory up
		delete printPort;
	}

	free(msg);
	delete routingPrtcl;
}
