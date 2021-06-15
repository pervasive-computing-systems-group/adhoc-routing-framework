#include "sequential_logging_manager.h"

SequentialLoggingDataManager::SequentialLoggingDataManager(string logFile, uint32_t loggingRate)
:LoggingDataManager(logFile, loggingRate){}

SequentialLoggingDataManager::SequentialLoggingDataManager(string logFile, uint32_t loggingRate, vector<DataCreator*> dataCreators):
LoggingDataManager(logFile, loggingRate, dataCreators){}

string SequentialLoggingDataManager::_getData(){
    if(this->dataCreators.empty()){
        return "";
    }
    
    // Ensure index is within list size
    this->index %= this->dataCreators.size();
    // Grab the data
    string data = this->dataCreators.at(this->index)->getData();
    // increment index
    this->index ++;

    return data;
}