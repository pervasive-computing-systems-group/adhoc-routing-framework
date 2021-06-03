#ifndef _DATA_CREATOR_H
#define _DATA_CREATOR_H
#include <string>

using namespace std;

/**
 * @brief A base class for objects which create fake data to send over the network
 * 
 */
class DataCreator{
public:
    DataCreator(){};

    virtual string getData() = 0;
};

#endif