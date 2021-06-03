#ifndef _GPS_CREATOR_H
#define _GPS_CREATOR_H
#include "data_creator.h"
#include <sstream>

class GPSCreator: public DataCreator{
public:
    GPSCreator();

    string getData() override;
};

#endif