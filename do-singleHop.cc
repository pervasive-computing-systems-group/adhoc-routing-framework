//
// do-singleHop.c runs a basic test of the TCP single-hop approach
//
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <fstream>
#include "hardware/hardware_single_hop.h"
#include "adhoc/print_port.h"

int main(){
	std::string sAP_IP = "192.168.1.2";
	const int str_leng = 16;
	HardwareSingleHop hSH(MY_IP_ADDR);
	PrintPort printPort(DATA_PORT);
	hSH.addPort(&printPort);

	char msg[str_leng];
	std::string message = "Hello World!";
	memcpy(msg, &message, str_leng);

	while(true) {
		// Send hello packet to access point node
		uint32_t dest = getIpFromString(sAP_IP);
		if(!hSH.sendPacket(printPort.getPortId(), msg, str_leng, dest)) {
			fprintf(stderr, "[DO TCP]:[ERROR]: Oof.. something went wrong...\n");
		}

		printf("[DO TCP]:[DEBUG]: Sent ");
		printPacket(stdout, msg, message.length());
		printf(" to %s\n", sAP_IP.c_str());

		// Handle packets
		int handleCount = hSH.handlePackets();
		if(handleCount > 0) {
			printf("[DO TCP]:[DEBUG]:Handled %d TCP packets\n", handleCount);
		}
	}
}
