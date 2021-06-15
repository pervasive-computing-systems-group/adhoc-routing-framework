#ifndef _STRING_CREATOR_H
#define _STRING_CREATOR_H

#include <iostream>
#include <fstream>
#include "data_creator.h"

class StringCreator: public DataCreator{
public:
    StringCreator(int size);
    string getData() override;

protected:
    int size;
};

#endif