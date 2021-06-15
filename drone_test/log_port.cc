#include "log_port.h"

LogPort::LogPort(int portId, string fileName, uint32_t logRate): Port(portId), numDataReceived(0), sizeDataReceived(0), logFile(fileName), loggingRate(logRate){
    lastLogTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}

void LogPort::handlePacket(char* data, int length, IP_ADDR source){
    // Increment information
    this->numDataReceived ++;
    this->sizeDataReceived += length;

    // Log the data every loggingRate milliseconds
    chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    if((currentTime - lastLogTime).count() > this->loggingRate){
        this->lastLogTime = this->lastLogTime + chrono::milliseconds(this->loggingRate);

        this->_logData();
    }
}

void LogPort::_logData(){
    // Open the file (append)
    ofstream fout(this->logFile, ios::app);
    if(!fout.is_open()){
        fprintf(stderr, "[LOGGING DATA MANAGER]:[ERROR]: Unable to open file for logging\n");
    }
    
    // Log info
    chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    fout << currentTime.count() << " " << this->numDataReceived << " " << this->sizeDataReceived << endl;
    
    // Close
    fout.close();
}