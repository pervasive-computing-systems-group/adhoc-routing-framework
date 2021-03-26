#include "hardware_hello_aodv.h"

void HardwareHelloAODV::_hardwareHelloAODV() {
  // Create hello monitor
  helloMonitor = new HardwareHelloMonitor(HELLO_PORT, this);
  // Add to ports
  addPort(helloMonitor);
  // Thread the sending of hello messages
  helloing = thread(&HardwareHelloAODV::_sendHelloThreaded, this);
  helloing.detach();
}

void HardwareHelloAODV::_hardwareHelloAODV(uint64_t helloDurationMS) {
  // Create hello monitor
  helloMonitor = new HardwareHelloMonitor(HELLO_PORT, this);
  // Add to ports
  addPort(helloMonitor);
  // Thread the sending of hello messages
  helloing = thread(&HelloMonitor::sendHellos, helloMonitor, helloDurationMS);
  helloing.detach();
}

HardwareHelloAODV::HardwareHelloAODV() : HardwareAODV() {
  _hardwareHelloAODV();
}

HardwareHelloAODV::HardwareHelloAODV(uint32_t ip) : HardwareAODV(ip) {
  _hardwareHelloAODV();
}

HardwareHelloAODV::HardwareHelloAODV(const char* ip) : HardwareAODV(ip) {
  _hardwareHelloAODV();
}

HardwareHelloAODV::HardwareHelloAODV(uint32_t ip, uint64_t helloDurationMS)
    : HardwareAODV(ip) {
  _hardwareHelloAODV(helloDurationMS);
}

HardwareHelloAODV::HardwareHelloAODV(const char* ip, uint64_t helloDurationMS)
    : HardwareAODV(ip) {
  _hardwareHelloAODV(helloDurationMS);
}

HardwareHelloAODV::~HardwareHelloAODV() { delete helloMonitor; }

int HardwareHelloAODV::handlePackets() {
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
    }
  }
  return count;
}

void HardwareHelloAODV::_sendHelloThreaded() {
  while (true) {
    helloMonitor->sendHellos(HELLO_NEIGHBOR_TTL_MS * 100);
  }
}