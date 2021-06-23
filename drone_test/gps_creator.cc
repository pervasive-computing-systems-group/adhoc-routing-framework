#include "gps_creator.h"

GPSCreator::GPSCreator(){

}

char* GPSCreator::getData(int* data_size) {
	// Generate random lat-long measurements
	float lat = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/180)) - 90.0;
	float lon = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/360)) - 180.0;
	stringstream sstream;
	sstream << "(" << lat << ", " << lon << ")";

	*data_size = sstream.str().size();
	char* ret_data = (char*) malloc(*data_size);
	memcpy(ret_data, sstream.str().c_str(), *data_size);

	return ret_data;
}
