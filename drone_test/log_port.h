#ifndef _LOG_PORT_H
#define _LOG_PORT_H

#include <string>
#include <fstream>
#include <chrono>

#include "port.h"
#include "data_logger.h"

class LogPort : public Port{
public:
    LogPort(int portId, string fileName, uint32_t logRate);

    void handlePacket(char* data, int length, IP_ADDR source);

private:
    DataLogger dataLogger;
   
};

#endif