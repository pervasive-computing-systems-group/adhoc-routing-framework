// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>

#include "routing_protocol.h"
#include "hardware_hello_aodv.h"
#include "data_manager.h"
#include "random_data_manager.h"
#include "image_creator.h"
#include "gps_creator.h"
#include "hardware_sh_ap.h"
#include "hardware_sh_station.h"
#include "log_port.h"

using namespace std;

int main(){
	// Seed random
	srand(time(0));

	/// Networking Settings
	vector<string> ips = { "192.168.1.1" };
	RoutingProtocol* routingPrtcl;
	DataManager* dataManager = new RandomDataManager();
	
	// Add data creators
	ImageCreator imageCreator;
	GPSCreator gpsCreator;
	dataManager->addDataCreator(&imageCreator);
	dataManager->addDataCreator(&gpsCreator);

	// Logging


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
						string message = dataManager->getData();
						char* msg = strdup(message.c_str());
						if(routingPrtcl->sendPacket(DATA_PORT, msg, message.length() + 1, dest) == -1){
							fprintf(stderr, "[TEST ADHOC]:[ERROR]: Unable to send packet\n");
						}
						free(msg);
					}
				}

				routingPrtcl->handlePackets();
			}
		}
	}
	else if(RT_PROTOCOL == USE_AODV) {
		/// Setup
		printf("[TEST ADHOC]: Using AODV\n");
		routingPrtcl = new HardwareHelloAODV(MY_IP_ADDR);
        // TODO: Create data loging port
		Port* logPort = new LogPort(DATA_PORT, "test_logs/aodv_test_1_received.txt", 10);
		routingPrtcl->addPort(logPort);

		// Network
		std::chrono::milliseconds last_send_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		while(true) {
			std::chrono::milliseconds current_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

			if((current_time - last_send_time).count() > 1000){
				last_send_time = current_time;
				for(auto ip : ips){
					uint32_t dest = getIpFromString(ip);
					string message = dataManager->getData();
					char* msg = strdup(message.c_str());
					if(routingPrtcl->sendPacket(logPort->getPortId(), msg, message.length()+1, dest) == -1){
						printf("[TEST ADHOC]:[DEBUG]: Unable to send packet (not connected or an error)\n");
					}
					free(msg);
				}
			}

			// Handle packets
			int handleCount = routingPrtcl->handlePackets();
		}

		// Clean memory up
		delete logPort;
	}
	delete routingPrtcl;
}