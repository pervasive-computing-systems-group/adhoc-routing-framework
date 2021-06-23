// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>

#include "defines.h"
#include "routing_protocol.h"
#include "hardware_hello_aodv.h"
#include "image_creator.h"
#include "gps_creator.h"
#include "string_creator.h"
#include "hardware_sh_ap.h"
#include "hardware_sh_station.h"
#include "log_port.h"
#include "los_connection_handler.h"
#include "logging_packet_handler.h"
#include "logging_aodv.h"

using namespace std;

int main() {
	// Seed random the same
	srand(0);

	// Logging Settings			 
	// File names (pattern: test_logs/test<test_num>/<routing_protocol>_<ip>_data_<metric>.txt)
	string dataReceivedFileName = "test_logs/test1/aodv_1_data_received.txt";
	string dataSentFileName = "test_logs/test1/aodv_1_data_sent.txt";

	/// Networking Settings
	vector<string> ips = SENDTO_LIST; // Who to send data to
	std::chrono::milliseconds dataLapse = std::chrono::milliseconds(DATA_RATE); // how long to wait between sending data
	std::chrono::milliseconds bufferLapse = std::chrono::milliseconds(BUFF_CHECK_RATE); // how long to wait between sending buffered data
	/// main loop to read/send packets
	std::chrono::milliseconds startTime
		= std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

	RoutingProtocol* routingPrtcl;
	
	/// Add data creators
	DataCreator* dataCreator;
	// Send images
//	dataCreator = new ImageCreator("image1.jpg");
	// Send GPS data
//	dataCreator = new GPSCreator();
	// Send random, long strings of data
	dataCreator = new StringCreator(MAXLINE - HEADER_SIZE - 1);

	if(DRONE_TEST_DEBUG) {
		cout << "[DRONE TEST]: Data creators initialized" << endl;
	}

	// LOS connection handler, give it ip list and asteroid-centered orbits
	LOSConnectionHandler losConnectionHandler(getIpFromString(MY_IP_ADDR), "test_data/test1/ac_flight_data.orb", "test_data/test1/ip_map.txt");
	if(DRONE_TEST_DEBUG) {
		cout << "[DRONE TEST]: App connection handler initialized" << endl;
	}

	if(RT_PROTOCOL == USE_SINGLE_HOP) {
		printf("[DRONE TEST]: Using SINGLE-HOP, ");
		/// Setup routing protocol
		LoggingAppPacketHandler loggingPacketHandler(dataSentFileName, dataReceivedFileName, LOGGING_RATE);

		if(SH_NODE_TYPE == AP_NODE) {
			printf("as access point!\n");
			// Create routing protocol using LED_APH_SHData app
			routingPrtcl = new HardwareSHAP(MY_IP_ADDR, DATA_PORT, &loggingPacketHandler);
			routingPrtcl->setAppConnectionHandler(&losConnectionHandler);

			/// Main loop to receive packets. The AP runs continuously.
			while(true) {
				// Read packets, access point doesn't send anything
				routingPrtcl->handlePackets();
			}
		}
		else if(SH_NODE_TYPE == STATION_NODE) {
			printf("as station!\n");
			routingPrtcl = new HardwareSHStation(MY_IP_ADDR, DATA_PORT, &loggingPacketHandler);
			routingPrtcl->setAppConnectionHandler(&losConnectionHandler);

			/// main loop to read/send packets
			std::chrono::milliseconds lastSendTime
				= std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
			std::chrono::milliseconds lastBuffTime = lastSendTime;
			int last_sent = 0;

			// Send a packet every second
			std::chrono::milliseconds currentTime
				= std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
			while((currentTime - startTime).count() >= MISSION_DURATION) {
				if((currentTime - lastSendTime) > dataLapse) {
					lastSendTime += dataLapse;
					for(auto ip : ips) {
						uint32_t dest = getIpFromString(ip);
						int msg_length = 0;
						char* msg = dataCreator->generateData(&msg_length);

						if(DRONE_TEST_DEBUG) {
							printf("[DRONE TEST]:[DEBUG]: Sending message of size: %d bytes to %s\n", msg_length, ip.c_str());
						}
						
						routingPrtcl->sendPacket(DATA_PORT, msg, msg_length, dest);
						
						free(msg);
					}
				}

				// Periodically check packet buffer
				if(((currentTime - lastBuffTime) > bufferLapse) || (last_sent)) {
					lastBuffTime = currentTime;
					last_sent = routingPrtcl->emptyBuffer();
				}

				routingPrtcl->handlePackets();

				currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
			}

			// Verify that all iterations of data generation are complete, but don't send anything
			while((currentTime - lastSendTime) > dataLapse) {
				lastSendTime += dataLapse;
				for(auto ip : ips) {
					int msg_length = 0;
					char* msg = dataCreator->generateData(&msg_length);
					free(msg);
				}
			}
		}
	}
	else if(RT_PROTOCOL == USE_AODV) {
		/// Setup
		if(DRONE_TEST_DEBUG) {
			printf("[DRONE TEST]: Using AODV\n");
		}
		routingPrtcl = new LoggingAODV(MY_IP_ADDR, dataSentFileName, LOGGING_RATE);
		routingPrtcl->setAppConnectionHandler(&losConnectionHandler);

        // Create data loging port
		LogPort logPort(DATA_PORT, dataReceivedFileName, 10);
		routingPrtcl->addPort(&logPort);

		// Network
		std::chrono::milliseconds lastSendTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		std::chrono::milliseconds lastBuffTime = lastSendTime;
		int last_sent = 0;

		// Periodically send data packets
		std::chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		while((currentTime - startTime).count() >= MISSION_DURATION) {
			if((currentTime - lastSendTime) > dataLapse) {
				lastSendTime += dataLapse;
				for(auto ip : ips){
					uint32_t dest = getIpFromString(ip);
					int msg_length = 0;
					char* msg = dataCreator->generateData(&msg_length);

					if(DRONE_TEST_DEBUG) {
						printf("[DRONE TEST]:[DEBUG]: Sending message of size: %d bytes to %s\n", msg_length, ip.c_str());
					}

					if(routingPrtcl->sendPacket(logPort.getPortId(), msg, msg_length, dest) == -1) {
						if(DRONE_TEST_DEBUG) {
							printf("[DRONE TEST]:[DEBUG]: Unable to send packet (not connected or an error)\n");
						}
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
			if(DRONE_TEST_DEBUG) {
				printf("[DRONE TEST]:[DEBUG]: handled %d messages\n", handleCount);
			}

			currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		}

		// Verify that all iterations of data generation are complete, but don't send anything
		while((currentTime - lastSendTime) > dataLapse) {
			lastSendTime += dataLapse;
			for(auto ip : ips) {
				int msg_length = 0;
				char* msg = dataCreator->generateData(&msg_length);
				free(msg);
			}
		}
	}

	delete dataCreator;
	delete routingPrtcl;
}
