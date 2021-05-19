#include "hardware_led_single_hop.h"

HardwareLedSingleHop::HardwareLedSingleHop() : HardwareLedSingleHop() {}

HardwareLedSingleHop::HardwareLedSingleHop(uint32_t ip) : HardwareLedSingleHop(ip) {}

HardwareLedSingleHop::HardwareLedSingleHop(const char* ip) : HardwareLedSingleHop(ip) {}

HardwareLedSingleHop::HardwareLedSingleHop(uint32_t ip, uint64_t helloDurationMS)
    : HardwareLedSingleHop(ip) {}

HardwareLedSingleHop::HardwareLedSingleHop(const char* ip, uint64_t helloDurationMS)
    : HardwareLedSingleHop(ip) {}

HardwareLedSingleHop::~HardwareLedSingleHop() {}

int HardwareLedSingleHop::handlePackets() {
	dicks
	/*
  int aodvCount = 0;
  int helloCount = 0;
  int otherCount = 0;
  int count = 0;

  Message message;
  // Handle aodv packets
  while (aodvSocket->getMessage(message)) {
    char* messageData = message.getData();
    _handleAODVPacket(messageData, message.getLength(),
                      message.getAddressI());
    helloMonitor->receiveHelloMessage(message.getAddressI());
    free(messageData);
    count++;
    
    // Light up the led if not a message from ourselves
    if(message.getEndpoint().getAddressI() != this->ipAddress){
      aodvCount ++;
      if(LED_AODV_DEBUG)
        printf("[LED AODV]:[DEBUG]: Received AODV message from %s\n", message.getEndpoint().getAddressC());
    }else{
      if(LED_AODV_DEBUG)
        printf("[LED AODV]:[DEBUG]: Received AODV message from self\n");
    }
  }

  // Handle packets on the ports
  for (auto socketPair : portSockets) {
    while (socketPair.second->getMessage(message)) {
      char* messageData = message.getData();
      this->_handlePacket(socketPair.first, messageData,
                          message.getLength(), message.getAddressI());
      helloMonitor->receiveHelloMessage(message.getAddressI());
      free(messageData);
      count++;

      // Light hello led if hello
      if(socketPair.first == HELLO_PORT){
        if(message.getEndpoint().getAddressI() != this->ipAddress){
          helloCount++;
          if(LED_AODV_DEBUG)
            printf("[LED AODV]:[DEBUG]: Received HELLO message from %d (self: %d)\n", message.getEndpoint().getAddressI(), this->getIp());
        }else{
          if(LED_AODV_DEBUG)
            printf("[LED AODV]:[DEBUG]: Received HELLO message from self\n");
        }
      }else{
        if(message.getEndpoint().getAddressI() != this->ipAddress){
          otherCount ++;
          if(LED_AODV_DEBUG)
            printf("[LED AODV]:[DEBUG]: Received DATA message from %s\n", message.getEndpoint().getAddressC());
        }else{
          if(LED_AODV_DEBUG)
            printf("[LED AODV]:[DEBUG]: Received message from self\n");
        }
      }
    }
  }

  // Flash the LEDS
  if(AODV_LED_ON && aodvCount)
    lightLed(AODV_LED_PIN, LIGHT_MS, aodvCount);
  if(HELLO_LED_ON && helloCount)
    lightLed(HELLO_LED_PIN, LIGHT_MS, helloCount);
  if(OTHER_LED_ON && otherCount)
    lightLed(OTHER_LED_PIN, LIGHT_MS, otherCount);

  return count;
  */
	return 0;
}
