#ifndef _APP_CONNECTION_HANDLER_H
#define _APP_CONNECTION_HANDLER_H

#include "adhoc_defines.h"

/**
 * @brief Application specific connectivity,
 * does not decide full connection but can mask undesired connections
 * 
 */
class AppConnectionHandler{
public:
    AppConnectionHandler(IP_ADDR ip): m_ip(ip) {};
    virtual ~AppConnectionHandler() {}

    /**
     * @brief checks if we can send or receive packets from a specific ip
     * 
     * @param ip ip address to check connection with
     * @return true can send/receive
     * @return false 
     */
    virtual bool isConnected(IP_ADDR ip) = 0;

protected:
    IP_ADDR m_ip;
};

#endif