#ifndef _GPS_CREATOR_H
#define _GPS_CREATOR_H
#include "data_creator.h"
#include <sstream>

class GPSCreator: public DataCreator{
public:
    GPSCreator();

protected:
	/*
	 * Generates GPS data. Returns a pointer to the data and stores size of data in
	 *  data_size.
	 */
    char* getData(int* data_size) override;
};

#endif
