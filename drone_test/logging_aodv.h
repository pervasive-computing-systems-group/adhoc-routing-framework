#ifndef _LOGGING_AODV_H
#define _LOGGING_AODV_H

#include "hardware_hello_aodv.h"
#include "data_logger.h"

class LoggingAODV : public HardwareHelloAODV{
public:
    /*!
     * @brief Construct a new HardwareHelloAODV object
     * 
     * @param ip the ip address of the current device
     */
    LoggingAODV(uint32_t ip, string logFile, uint32_t loggingRate);
    LoggingAODV(const char* ip, string logFile, uint32_t loggingRate);

protected:
    DataLogger receiveLogger;

    virtual int protocolSendPacket(int portId, char* packet, int length, IP_ADDR dest, IP_ADDR origIP = -1) override;
};

#endif