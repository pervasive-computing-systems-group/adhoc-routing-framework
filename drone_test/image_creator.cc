#include "image_creator.h"

ImageCreator::ImageCreator(string filename):filename(filename){

}

char* ImageCreator::getData(int* data_size) {
	// Copy image description into string
	ifstream myfile (this->filename, ios_base::binary);
	string image;

	if(myfile.is_open()) {
		string line;
		while(getline(myfile, line)) {
			image.append(line);
		}

		myfile.close();
	}
	else {
		fprintf(stderr, "[IMAGE CREATOR]:[ERROR]: unable to open image file\n");
	}

	// Copy string characters into byte array
	*data_size = image.size();
	char* ret_data = (char*) malloc(*data_size);
	memcpy(ret_data, image.c_str(), *data_size);

	return ret_data;
}
