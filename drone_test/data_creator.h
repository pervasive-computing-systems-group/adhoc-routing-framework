#pragma once

#include <cstdint>
#include <cstddef>
#include <string.h>

#include "data_logger.h"

/**
 * @brief A base class for objects which create fake data to send over the network
 * 
 */
class DataCreator {
public:
	DataCreator();
	virtual ~DataCreator();

	/*
	 * Generates dynamic data based on derived class specifications. Returns a pointer to the dynamically allocated
	 *  memory and stores the size of the data in data_size. Runs data logging on created data.
	 *
	 * NOTE: This will dynamically allocate memory. When done using this data, user must call free to
	 *  release memory allocation.
	 */
	char* generateData(int* data_size);
protected:

	/*
	 * Derived class specific data generation. Returns a pointer to the data and stores size of data in
	 *  data_size.
	 */
	virtual char* getData(int* data_size) = 0;

private:
	DataLogger m_oLogger;
};
