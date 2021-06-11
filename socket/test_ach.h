#ifndef _TEST_ACH_H
#define _TEST_ACH_H

#include "app_connection_handler.h"

class TestACH : public AppConnectionHandler{
    public:
    TestACH(){}

    bool isConnected(uint32_t) override{
        return false;
    }
};

#endif