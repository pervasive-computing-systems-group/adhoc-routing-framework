#include "random_data_manager.h"

RandomDataManager::RandomDataManager(){
    
}

RandomDataManager::RandomDataManager(vector<DataCreator*> dataCreators) : DataManager(dataCreators){

}

string RandomDataManager::getData(){
    return this->dataCreators.at(rand()%this->dataCreators.size())->getData();
}