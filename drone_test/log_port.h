#ifndef _LOG_PORT_H
#define _LOG_PORT_H

#include <string>
#include <fstream>
#include "port.h"

class LogPort : public Port{
public:
    LogPort(int portId, string fileName, uint32_t logRate);

    void handlePacket(char* data, int length, IP_ADDR source);

private:
    void _logNumPackets();

    int logRate;
    int counter;
    string logFile;
};

#endif