#ifndef _LOGGING_PACKET_HANDLER_H
#define _LOGGING_PACKET_HANDLER_H

#include <string>
#include <chrono>
#include <fstream>

#include "app_packet_handler.h"
#include "data_logger.h"

using namespace std;

class LoggingAppPacketHandler : public AppPacketHandler {
public:
    LoggingAppPacketHandler(string sentFilename, string receivedFilename, uint32_t logRate);

	/**
	 * @brief Receives a packet and logs how many and how much data it gets
	 * 
	 * @param pMsg 
	 */
	virtual void runReceiveHandler(Message* pMsg) override;

	/**
	 * @brief Receives a packet and logs how many and how much data it sends
	 * 
	 * @param pMsg 
	 */
	virtual void runSendHandler(int nBytesSent, char* pMsg) override;

protected:
	DataLogger sendLogger;
    DataLogger receiveLogger;
    
};

#endif