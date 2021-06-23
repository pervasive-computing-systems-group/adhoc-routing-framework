#include "logging_packet_handler.h"

LoggingAppPacketHandler::LoggingAppPacketHandler(string sentFilename, string receivedFilename, uint32_t logRate)
	: sendLogger(sentFilename, logRate),
	  receiveLogger(receivedFilename, logRate)
{ }

void LoggingAppPacketHandler::runReceiveHandler(Message* pMsg){
    receiveLogger.addData(pMsg->getData(), pMsg->getLength());
}

void LoggingAppPacketHandler::runSendHandler(int nBytesSent, char* pMsg){
    if(nBytesSent > 0){
    	sendLogger.addData(pMsg, nBytesSent);
    }
}
