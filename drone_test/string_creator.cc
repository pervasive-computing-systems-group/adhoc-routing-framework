#include "string_creator.h"

StringCreator::StringCreator(int size)
	: size(size)
{ }

char* StringCreator::getData(int* data_size) {
	// Generate a string of a's
	char* msg = strdup(string(size, 'a').c_str());
	*data_size = size;

	return msg;
}
