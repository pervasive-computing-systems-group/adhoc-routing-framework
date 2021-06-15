#include "log_port.h"

LogPort::LogPort(int portId, string fileName, uint32_t logRate): Port(portId), dataLogger(fileName, logRate){
}

void LogPort::handlePacket(char* data, int length, IP_ADDR source){
    dataLogger.addData(data, length);
}