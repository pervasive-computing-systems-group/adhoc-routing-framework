#ifndef _LOGGING_DATA_MANAGER_H
#define _LOGGING_DATA_MANAGER_H

#include <vector>
#include <fstream>
#include <chrono>

#include "data_manager.h"

using namespace std;

class LoggingDataManager : public DataManager{
public:
    // Constructor
    /**
     * @brief Construct a new Logging Data Manager object
     * 
     * @param logFile the file to log data to
     * @param loggingRate milliseconds between logging data
     */
    LoggingDataManager(string logFile, uint32_t loggingRate);
    /**
     * @brief Construct a new Logging Data Manager object
     * 
     * @param logFile the file to log data to
     * @param loggingRate milliseconds between logging data
     * @param dataCreators list of data creators 
     */
    LoggingDataManager(string logFile, uint32_t loggingRate, vector<DataCreator*> dataCreators);

    /**
     * @brief Gets a data string and logs info to file
     * 
     * @return string data from a generator
     */
    virtual string getData() override;

protected:
    long unsigned int numDataGenerated;
    long unsigned int sizeDataGenerated;

    string logFile;
    uint32_t loggingRate;
    std::chrono::milliseconds lastLogTime;

    /**
     * @brief logs current time, and the number of data points and total size of data points generated
     * 
     */
    void _logData();

    /**
     * @brief Returns the actual data from data generators, called by getData 
     * 
     * @return string 
     */
    virtual string _getData() = 0;
};

#endif