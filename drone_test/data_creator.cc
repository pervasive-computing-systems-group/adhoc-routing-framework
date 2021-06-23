#include "defines.h"
#include "data_creator.h"

DataCreator::DataCreator()
	: m_oLogger(DATA_CAPRUTE_LOG, LOGGING_RATE)
{ }

DataCreator::~DataCreator()
{ }

char* DataCreator::generateData(int* data_size) {
	char* ret_data = getData(data_size);
	m_oLogger.addData(ret_data, *data_size);

	return ret_data;
}
