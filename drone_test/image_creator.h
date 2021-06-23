#ifndef _IMAGE_CREATOR_H
#define _IMAGE_CREATOR_H

#include <iostream>
#include <fstream>
#include "data_creator.h"

class ImageCreator: public DataCreator{
public:
    ImageCreator(string filename);

protected:
	/*
	 * "Generates" an image. Returns a pointer to the data and stores size of data in
	 *  data_size.
	 */
    char* getData(int* data_size) override;

private:
    string filename;
};

#endif
