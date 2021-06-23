#include "data_logger.h"

DataLogger::DataLogger(string fileName, uint32_t logRate): numData(0), sizeData(0), logFile(fileName), loggingRate(logRate){
    lastLogTime = chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}
    
void DataLogger::addData(char* data, int length){
    if(length > 0){
        addData(string(data, length));
    }
}

// TODO: pelo amor de deus: STOP PASSING AROUND STRING OBJECTS!!!
void DataLogger::addData(string data){
    // Increment information
    this->numData ++;
    this->sizeData += data.length();

    // Log the data every loggingRate milliseconds
    chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    if((currentTime - lastLogTime).count() > this->loggingRate){
        this->lastLogTime = this->lastLogTime + chrono::milliseconds(this->loggingRate);

        this->_logData();
    }
}

void  DataLogger::_logData(){
    // Open the file (append)
    ofstream fout(this->logFile, ios::app);
    if(!fout.is_open()){
        fprintf(stderr, "[LOGGING DATA MANAGER]:[ERROR]: Unable to open file for logging\n");
    }
    
    // Log info
    chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    fout << currentTime.count() << " " << this->numData << " " << this->sizeData << endl;
    
    // Close
    fout.close();
}
