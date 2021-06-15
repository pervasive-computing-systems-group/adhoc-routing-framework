#include "logging_manager.h"

LoggingDataManager::LoggingDataManager(string logFile, uint32_t loggingRate) : dataLogger(logFile, loggingRate) {
}

LoggingDataManager::LoggingDataManager(string logFile, uint32_t loggingRate, vector<DataCreator*> dataCreators): DataManager(dataCreators), dataLogger(logFile, loggingRate){
}

string LoggingDataManager::getData(){
    // Get data from inherited child
    string data = this->_getData();

    dataLogger.addData(data);

    // Return the data
    return data;
}