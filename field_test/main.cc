// this file will do ad hoc! 
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>

#include "hardware_led_aodv.h"
#include "hardware_led_single_hop.h"
#include "aodv_test.h"

using namespace std;

int main(){
	/// Setup
	RoutingProtocol* routingPrtcl;
	switch(RT_PROTOCOL) {
	case USE_SINGLE_HOP:
		printf("[TEST ADHOC]: Using SINGLE-HOP\n");
		routingPrtcl = new HardwareLedSingleHop(MY_IP_ADDR);
		break;

	case USE_AODV:
		printf("[TEST ADHOC]: Using AODV\n");
		routingPrtcl = new HardwareLedAODV(MY_IP_ADDR);
		break;
	}
	Port* printPort = new PrintPort(DATA_PORT);
	routingPrtcl->addPort(printPort);

	// Light up all LEDs to avoid random lighting
	printf("[TEST ADHOC]:[DEBUG]: Cleaning LED pins...\n");
	lightLed(BLUE_LED_PIN, LED_HOLD);
	lightLed(YELLOW_LED_PIN, LED_HOLD);
	lightLed(RED_LED_PIN, LED_HOLD);
	printf("[TEST ADHOC]:[DEBUG]: Cleaning LED pins...done\n");

	/// Networking Settings
	string message = "Hello World!";
	char* msg = strdup(message.c_str());

	vector<string> ips = { "192.168.1.2" };

	// Network
	std::chrono::milliseconds last_send_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
	while(true) {
		// Send packets to all ips every second
		std::chrono::milliseconds current_time = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

		if((current_time - last_send_time).count() > 1000){
			last_send_time = current_time;
			for(auto ip : ips){
				uint32_t dest = getIpFromString(ip);
				if(!routingPrtcl->sendPacket(printPort->getPortId(), msg, message.length()+1, dest)){
					fprintf(stderr, "[TEST ADHOC]:[ERROR]: Unable to send packet\n");
					exit(1);
				}
				if(RT_PROTOCOL == USE_SINGLE_HOP) {
					// Signal that we attempted to send a packet
					lightLed(RED_LED_PIN, LED_HOLD);
				}
				printf("[TEST ADHOC]:[DEBUG]: Sent ");
				printPacket(stdout, msg, message.length()+1);
				printf(" to %s\n", ip.c_str());
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
	delete routingPrtcl;
	free(msg);
}
