#include "string_creator.h"

StringCreator::StringCreator(int size):size(size){

}

string StringCreator::getData(){
    return string(this->size, 'a');
}