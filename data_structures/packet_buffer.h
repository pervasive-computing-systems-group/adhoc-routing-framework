/*********************************
 * packet_buffer.h
 *
 * C++ implementation of a Packet Buffer
 *
 * Author: Jonathan Diller
 * Date: 08/17/2020
 ********************************/

#pragma once

#include <iostream>
#include <queue>
#include <deque>
#include <map>

#include "buffered_packet.h"
#include "defines.h"

class PacketBuffer
{
public:

	PacketBuffer();
	~PacketBuffer();

	/**
	 * @brief Creates a buffered packet and stores it
	 *
	 * @param destination: The final destination of the packet
	 * @param portID: The port number that the packet should go out
	 * @param buffer: A pointer to the packets data buffer
	 * @param length: The length of the data buffer
     * @return void
	 */
	void storePacket(IP_ADDR destination, int portId, char* buffer, int length);

	/**
	 * @brief Stores packets in buffer
	 *
	 * @param bufferedPacket: the packet to store
     * @return void
	 */
	void storePacket(BufferedPacket* bufferedPacket);

	/**
	 * @brief Checks to see if there are packets waiting to be sent to dest
	 *
	 * @param dest: the IP address of the destination
     * @return true if there is a packet waiting to be sent to this destination, false otherwise
	 */
	bool packetsWaiting(IP_ADDR dest);

	/**
	 * @brief Pops off the top packet for dest
	 *
	 * @param dest: the IP address of the destination
	 * @param bufferedPacket: a pointer to a buffered packet that will hold the popped-off packets data
     * @return true if there is a packet waiting to be sent to this destination, false otherwise
	 */
	bool getPacket(IP_ADDR dest, BufferedPacket* bufferedPacket);

	/**
	 * @brief Get a list of all destination IP addresses for packets waiting to send
	 *
     * @param list: The deque to store IP addresses for packets waiting to be sent
	 */
	void getReceiverList(std::deque<int>* list);

	/**
	 * @brief Get the number of available bytes in the buffer
	 *
     * @return The number of bytes available in the buffer
	 */
	long getAvailableBytes();

	/**
	 * @brief Get the number of packets in the buffer
	 *
     * @return The number of packets in the buffer
	 */
	long getNumbPackets();

protected:

private:
	long const m_nBufferSize;
	long m_nByteCount;
	// map of rreq ids and their corresponding packet to be sent once the route is established
	std::map<uint32_t, std::queue<BufferedPacket>> m_oPacketBuffer;
};
