#include "logging_aodv.h"

LoggingAODV::LoggingAODV(uint32_t ip, string logFile, uint32_t loggingRate)
:HardwareHelloAODV(ip), receiveLogger(logFile, loggingRate){

}
LoggingAODV::LoggingAODV(const char* ip, string logFile, uint32_t loggingRate)
:HardwareHelloAODV(ip), receiveLogger(logFile, loggingRate){

}


int LoggingAODV::protocolSendPacket(int portId, char* packet, int length, IP_ADDR dest, IP_ADDR origIP){
    int nBytesSent = HardwareHelloAODV::protocolSendPacket(portId, packet, length, dest, origIP);
    
    if(nBytesSent > 0 && portId == DATA_PORT && origIP == this->ipAddress){
        receiveLogger.addData(packet, nBytesSent);
    }

    return nBytesSent;
}

