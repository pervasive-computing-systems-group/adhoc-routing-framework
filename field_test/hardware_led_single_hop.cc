#include "hardware_led_single_hop.h"

HardwareLedSingleHop::HardwareLedSingleHop(uint32_t ip) : HardwareSingleHop(ip) {}

HardwareLedSingleHop::HardwareLedSingleHop(const char* ip) : HardwareSingleHop(ip) {}

HardwareLedSingleHop::~HardwareLedSingleHop() {}

int HardwareLedSingleHop::handlePackets() {
	int dataCount = 0;
	int otherCount = 0;
	int count = 0;
	Message message;

	// Handle packets on the ports
	for (auto socketPair : portSockets) {
		while (socketPair.second->getMessage(message)) {
			count++;
			char* messageData = message.getData();
			_handlePacket(socketPair.first, messageData,
					message.getLength(), message.getAddressI());

			// Track packets received
			if(socketPair.first == DATA_PORT) {
				if(LED_SH_DEBUG)
					printf("[LED AODV]:[DEBUG]: Received DATA message from %s\n", message.getEndpoint().getAddressC());
				dataCount++;
			}
			else {
				if(LED_SH_DEBUG)
					printf("[LED AODV]:[DEBUG]: Received random message (from self?)\n");
				otherCount++;
			}
		}
	}

	// Flash the LEDS
	if(AODV_LED_ON && dataCount)
		lightLed(AODV_LED_PIN, LIGHT_MS, dataCount);
	if(OTHER_LED_ON && otherCount)
		lightLed(OTHER_LED_PIN, LIGHT_MS, otherCount);

	return count;
}
