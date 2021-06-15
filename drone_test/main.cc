// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>

#include "routing_protocol.h"
#include "hardware_hello_aodv.h"
#include "data_manager.h"
#include "sequential_logging_manager.h"
#include "image_creator.h"
#include "gps_creator.h"
#include "string_creator.h"
#include "hardware_sh_ap.h"
#include "hardware_sh_station.h"
#include "log_port.h"
#include "los_connection_handler.h"

using namespace std;
int main(){
	// Seed random the same
	srand(0);

	// Logging Settings			 
	// File names (pattern: test_logs/test<test_num>/<routing_protocol>_<ip>_data_<metric>.txt)
	string dataCapturedFileName = "test_logs/test1/aodv_1_data_captured.txt";
	string dataReceivedFileName = "test_logs/test1/aodv_1_data_received.txt";
	string dataSentFileName = "test_logs/test1/aodv_1_data_sent.txt";
	uint32_t loggingRate = 2000; // milliseconds between logging data

	/// Networking Settings
	vector<string> ips = { "192.168.1.1" }; // Who to send data to
	std::chrono::milliseconds dataLapse = std::chrono::milliseconds(1000); // how long to wait between sending data
	std::chrono::milliseconds bufferLapse = std::chrono::milliseconds(100); // how long to wait between sending buffered data
	RoutingProtocol* routingPrtcl;
	DataManager* dataManager = new SequentialLoggingDataManager(dataCapturedFileName, loggingRate);	
	
	// Add data creators
	ImageCreator imageCreator("image1.jpg");
	GPSCreator gpsCreator;
	StringCreator strCreator(MAXLINE - HEADER_SIZE - 1);

	dataManager->addDataCreator(&imageCreator);
	dataManager->addDataCreator(&gpsCreator);
	dataManager->addDataCreator(&strCreator);
	cout << "[TEST ADHOC]: Data creators initialized" << endl;

	// App connection handler
	LOSConnectionHandler losConnectionHandler(getIpFromString(MY_IP_ADDR), "test_data/test1/ac_flight_data.orb", "test_data/test1/ip_map.txt");
	cout << "[TEST ADHOC]: App connection handler initialized" << endl;

	// Logging


	if(RT_PROTOCOL == USE_SINGLE_HOP) {
		printf("[TEST ADHOC]: Using SINGLE-HOP, ");
		/// Setup routing protocol
		// TODO: Create data port packet handler

		if(SH_NODE_TYPE == AP_NODE) {
			printf("as access point!\n");
			// Create routing protocol using LED_APH_SHData app
			routingPrtcl = new HardwareSHAP(MY_IP_ADDR, DATA_PORT, nullptr);
			routingPrtcl->setAppConnectionHandler(&losConnectionHandler);

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
			routingPrtcl->setAppConnectionHandler(&losConnectionHandler);

			/// main loop to read/send packets
			std::chrono::milliseconds lastSendTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
			std::chrono::milliseconds lastBuffTime = lastSendTime;
			int last_sent = 0;

			while(true) {
				// Send a packet every second
				std::chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

				if((currentTime - lastSendTime) > dataLapse) {
					lastSendTime += dataLapse;
					for(auto ip : ips) {
						uint32_t dest = getIpFromString(ip);
						string message = dataManager->getData();
						char* msg = strdup(message.c_str());
						
						routingPrtcl->sendPacket(DATA_PORT, msg, message.length() + 1, dest);
						
						free(msg);
					}
				}

				// Periodically check packet buffer
				if(((currentTime - lastBuffTime) > bufferLapse) || (last_sent)) {
					lastBuffTime = currentTime;
					last_sent = routingPrtcl->emptyBuffer();
				}

				routingPrtcl->handlePackets();
			}
		}
	}
	else if(RT_PROTOCOL == USE_AODV) {
		/// Setup
		printf("[TEST ADHOC]: Using AODV\n");
		routingPrtcl = new HardwareHelloAODV(MY_IP_ADDR);
		routingPrtcl->setAppConnectionHandler(&losConnectionHandler);

        // Create data loging port
		LogPort logPort(DATA_PORT, "test_logs/aodv_test_1_received.txt", 10);
		routingPrtcl->addPort(&logPort);

		// Network
		std::chrono::milliseconds lastSendTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		std::chrono::milliseconds lastBuffTime = lastSendTime;
		int last_sent = 0;

		while(true) {
			std::chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

			if((currentTime - lastSendTime) > dataLapse) {
				lastSendTime += dataLapse;
				for(auto ip : ips){
					uint32_t dest = getIpFromString(ip);
					string message = dataManager->getData();
					char* msg = strdup(message.c_str());

					if(routingPrtcl->sendPacket(logPort.getPortId(), msg, message.length()+1, dest) == -1){
						printf("[TEST ADHOC]:[DEBUG]: Unable to send packet (not connected or an error)\n");
					}

					free(msg);
				}
			}

			// Periodically check packet buffer
			if(((currentTime - lastBuffTime) > bufferLapse) || (last_sent)) {
				lastBuffTime = currentTime;
				last_sent = routingPrtcl->emptyBuffer();
			}

			// Handle packets
			int handleCount = routingPrtcl->handlePackets();
			//printf("[TEST ADHOC]:[DEBUG]: handled %d messages\n", handleCount);
		}
	}
	delete routingPrtcl;
}
