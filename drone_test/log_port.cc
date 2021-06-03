#include "log_port.h"

LogPort::LogPort(int portId, string fileName, uint32_t logRate): Port(portId), counter(0), logFile(fileName){
    // Set and check log rate
    if (logRate > 0){
        this->logRate = logRate;
    }else{
        fprintf(stderr, "[LOG PORT]:[ERROR]: cannot set log rate to zero");
        exit(-1);
    }
}

void LogPort::handlePacket(char* data, int length, IP_ADDR source){
    // Increment number of packets received
    this->counter ++;
    // If we have received enoug packets to log, log the packer
    if (this->counter % this->logRate){
        this->_logNumPackets();
    }
}

void LogPort::_logNumPackets(){
    // Open the file
    ofstream of(this->logFile);
    if(of.is_open()){
        of << this->counter;
    }else{
        fprintf(stderr, "[LOG PORT]:[ERROR]: Could not open file to log num packets");
    }
    of.close();
}