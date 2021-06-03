#ifndef _IMAGE_CREATOR_H
#define _IMAGE_CREATOR_H

#include <iostream>
#include <fstream>
#include "data_creator.h"

class ImageCreator: public DataCreator{
public:
    ImageCreator();
    string getData() override;
};

#endif