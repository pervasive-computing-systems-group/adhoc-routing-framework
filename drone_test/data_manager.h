#ifndef _DATA_MANAGER_H
#define _DATA_MANAGER_H

#include <vector>
#include "data_creator.h"
using namespace std;

class DataManager{
public:
    // Constructor
    DataManager();
    DataManager(vector<DataCreator*> dataCreators);

    void addDataCreator(DataCreator* dataCreator);

    virtual string getData() = 0;

protected:
    vector<DataCreator*> dataCreators;
};

#endif