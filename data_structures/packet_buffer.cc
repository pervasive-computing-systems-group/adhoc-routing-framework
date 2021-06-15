#include "packet_buffer.h"

PacketBuffer::PacketBuffer() : m_nBufferSize(PACKET_BUFFER_CAPACITY)
{
	m_nByteCount = 0;
}


PacketBuffer::~PacketBuffer()
{ }

/******************************
 * Public Functions
 ******************************/

void PacketBuffer::storePacket(IP_ADDR destination, int portId, char* buffer, int length, IP_ADDR origin) {
	BufferedPacket packet(destination, portId, buffer, length, origin);
	storePacket(&packet);
}

void PacketBuffer::storePacket(BufferedPacket* bufferedPacket)
{
	uint32_t dest = bufferedPacket->getDestination();
	// Check if there is a queue for this address
	if(!(m_oPacketBuffer.count(dest) > 0))
	{
		m_oPacketBuffer.insert(std::pair<uint32_t, std::queue<BufferedPacket>>(dest, std::queue<BufferedPacket>()));
	}

	do
	{
		// Try to add packet
		if((m_nByteCount + bufferedPacket->getLength()) <= m_nBufferSize)
		{
			// Add packet
			m_oPacketBuffer.at(dest).push(*bufferedPacket);
			m_nByteCount += bufferedPacket->getLength();

			break;
		}
		else
		{
			// Buffer is full, try to remove older packet
			if(m_oPacketBuffer.at(dest).size() > 0)
			{
				// Throw away oldest packet waiting to go to this destination
				m_nByteCount -= m_oPacketBuffer.at(dest).front().getDestination();
				m_oPacketBuffer.at(dest).pop();
			}
			else
			{
				// Buffer is full and there are no old packets to throw away for this destination
				// Just throw away this packet
				break;
			}
		}
	} while (true);
}

bool PacketBuffer::packetsWaiting(IP_ADDR dest)
{
	if(m_oPacketBuffer.count(dest) > 0)
	{
		return (m_oPacketBuffer.at(dest).size() > 0);
	}
	else
	{
		return false;
	}
}

// Make this function take a BuffPacket and return t/f
bool PacketBuffer::getPacket(IP_ADDR dest, BufferedPacket* bufferedPacket)
{
	bool ret_val = false;

	if(m_oPacketBuffer.count(dest) > 0)
	{
		if(m_oPacketBuffer.at(dest).size() > 0)
		{
			bufferedPacket->copyPacket(m_oPacketBuffer.at(dest).front());
			m_oPacketBuffer.at(dest).pop();	// This call the destructor on the stored packet!
			m_nByteCount -= bufferedPacket->getLength();
			ret_val = true;
		}
	}

	return ret_val;
}


// Get a list of all destination IP addresses for packets waiting to send
void PacketBuffer::getReceiverList(std::deque<int>* list) {
	for(auto it = m_oPacketBuffer.begin(); it != m_oPacketBuffer.end(); ++it)
	{
		list->push_back(it->first);
	}
}

// Get the number of available bytes in the buffer
long PacketBuffer::getAvailableBytes()
{
	return m_nBufferSize - m_nByteCount;
}

long PacketBuffer::getNumbPackets() {
	long retVal = 0;
	for(auto it = m_oPacketBuffer.begin(); it != m_oPacketBuffer.end(); ++it)
	{
		std::queue<BufferedPacket> packetQueue = it->second;
		retVal += packetQueue.size();
	}

	return retVal;
}

/******************************
 * Protected Functions
 ******************************/


/******************************
 * Private Functions
 ******************************/
