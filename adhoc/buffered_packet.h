#ifndef BUFFEREDPACKET_H
#define BUFFEREDPACKET_H

/*********************************
 * aodv.h
 *
 * Custom c++ implementation of the aodv routing protocol. 
 * 
 * Author: Josh Rands
 * Date: 9/4/2019
 ********************************/

class BufferedPacket{
    public:
        BufferedPacket(int portId, char* buffer, int length);
        ~BufferedPacket();
        
        int getPortId();
        char* getBuffer();
        int getLength();

    protected:
	    int portId;
	    char* buffer;
	    int length;
};


#endif