#ifndef _RANDOM_DATA_MANAGER_H
#define _RANDOM_DATA_MANAGER_H

#include "data_manager.h"
#include <cstdlib>

class RandomDataManager : public DataManager{
public:
    RandomDataManager();
    RandomDataManager(vector<DataCreator*> dataCreators);

    string getData() override;
};

#endif