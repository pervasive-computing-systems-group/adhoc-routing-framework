#ifndef _DATA_LOGGER_H
#define _DATA_LOGGER_H

#include <chrono>
#include <fstream>
#include <string>

using namespace std;

class DataLogger{
public:
    DataLogger(string fileName, uint32_t logRate);
    
    void addData(char* data, int length);
    void addData(string data);

protected:
    virtual void _logData();

    long unsigned int numData;
    long unsigned int sizeData;

    string logFile;
    uint32_t loggingRate;
    chrono::milliseconds lastLogTime;
};

#endif