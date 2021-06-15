#include "random_data_manager.h"

RandomDataManager::RandomDataManager(){
    
}

RandomDataManager::RandomDataManager(vector<DataCreator*> dataCreators) : DataManager(dataCreators){

}

string RandomDataManager::getData(){
    if(this->dataCreators.empty()) return "";
    else return this->dataCreators.at(rand()%this->dataCreators.size())->getData();
}