#include "data_manager.h"

DataManager::DataManager(){

}

DataManager::DataManager(vector<DataCreator*> dataCreators){
    this->dataCreators = dataCreators;
}

void DataManager::addDataCreator(DataCreator* dataCreator){
    this->dataCreators.push_back(dataCreator);
}