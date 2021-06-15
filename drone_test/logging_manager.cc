#include "logging_manager.h"

LoggingDataManager::LoggingDataManager(string logFile, uint32_t loggingRate) : numDataGenerated(0), sizeDataGenerated(0), logFile(logFile), loggingRate(loggingRate) {
    lastLogTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}

LoggingDataManager::LoggingDataManager(string logFile, uint32_t loggingRate, vector<DataCreator*> dataCreators): DataManager(dataCreators), numDataGenerated(0), sizeDataGenerated(0), logFile(logFile), loggingRate(loggingRate){
    lastLogTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}

string LoggingDataManager::getData(){
    // Get data from inherited child
    string data = this->_getData();

    // Increment information
    this->numDataGenerated ++;
    this->sizeDataGenerated += data.length();

    // Log the data every loggingRate milliseconds
    chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    if((currentTime - lastLogTime).count() > this->loggingRate){
        this->lastLogTime = this->lastLogTime + chrono::milliseconds(this->loggingRate);

        this->_logData();
    }

    // Return the data
    return data;
}

void LoggingDataManager::_logData(){
    // Open the file (append)
    ofstream fout(this->logFile, ios::app);
    if(!fout.is_open()){
        fprintf(stderr, "[LOGGING DATA MANAGER]:[ERROR]: Unable to open file for logging\n");
    }
    
    // Log info
    chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    fout << currentTime.count() << " " << this->numDataGenerated << " " << this->sizeDataGenerated << endl;
    
    // Close
    fout.close();
}
