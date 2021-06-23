#ifndef _STRING_CREATOR_H
#define _STRING_CREATOR_H

#include <iostream>
#include <fstream>

#include "data_creator.h"

class StringCreator: public DataCreator{
public:
    StringCreator(int size);

protected:
	/*
	 * Generates a string. Returns a pointer to the data and stores size of data in
	 *  data_size.
	 */
    char* getData(int* data_size) override;

private:
    int size;
};

#endif
