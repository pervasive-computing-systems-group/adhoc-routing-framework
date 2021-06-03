#include "gps_creator.h"

GPSCreator::GPSCreator(){

}

string GPSCreator::getData(){
    float lat = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/180)) - 90.0;
    float lon = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/360)) - 180.0;
    stringstream sstream;
    sstream << "(" << lat << ", " << lon << ")";
    return sstream.str();
}