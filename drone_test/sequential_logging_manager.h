#ifndef _SEQUENTIAL_LOGGING_DATA_MANAGER_H
#define _SEQUENTIAL_LOGGING_DATA_MANAGER_H

#include <vector>
#include <fstream>
#include <chrono>

#include "logging_manager.h"

using namespace std;

class SequentialLoggingDataManager : public LoggingDataManager{
public:
    // Constructor
    /**
     * @brief Construct a new Logging Data Manager object
     * 
     * @param logFile the file to log data to
     * @param loggingRate milliseconds between logging data
     */
    SequentialLoggingDataManager(string logFile, uint32_t loggingRate);
    /**
     * @brief Construct a new Logging Data Manager object
     * 
     * @param logFile the file to log data to
     * @param loggingRate milliseconds between logging data
     * @param dataCreators list of data creators 
     */
    SequentialLoggingDataManager(string logFile, uint32_t loggingRate, vector<DataCreator*> dataCreators);

protected:
    uint32_t index;

    /**
     * @brief everytime it is called it grabs data from the next data generator in the list
     * 
     * @return string 
     */
    virtual string _getData() override;
};

#endif