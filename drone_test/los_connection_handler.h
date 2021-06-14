#ifndef _LOS_CONNECTION_HANDLER_H
#define _LOS_CONNECTION_HANDLER_H

#include "app_connection_handler.h"
#include "line_of_sight.h"
#include "routing_protocol.h"
#include "drone_defines.h"

#include <string>
#include <chrono>
#include <unordered_map>
#include <fstream>


class LOSConnectionHandler: public AppConnectionHandler{
public:
    LOSConnectionHandler(uint32_t ip, string losFilename, string ipMapFilename);

    // The application specific function call for connection status
	virtual bool isConnected(uint32_t ip) override;

protected:
    void _pullIpMap(string ip_map_filename);

    IP_ADDR m_ip;
    unordered_map<IP_ADDR, int> m_mIpToId;
    LineOfSight m_los;
    std::chrono::milliseconds m_startTime;
};

#endif