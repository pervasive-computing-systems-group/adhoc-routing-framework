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
        BufferedPacket();
        BufferedPacket(const BufferedPacket &bufferedPacket);
        ~BufferedPacket();

        BufferedPacket& operator=(const BufferedPacket& bufferedPacket);
        
        int getPortId() const;
        char* getBuffer() const;
        int getLength() const;

    protected:
	    int portId;
	    char* buffer;
	    int length;
};


#endif