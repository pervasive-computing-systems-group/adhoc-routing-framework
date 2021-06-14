#include "image_creator.h"

ImageCreator::ImageCreator(string filename):filename(filename){

}

string ImageCreator::getData(){
    ifstream myfile (this->filename, ios_base::binary);
    string image;

    if(myfile.is_open()){
        string line;
        while(getline(myfile, line)){
            
            image.append(line);
        }
        myfile.close();
    }else{
        fprintf(stderr, "[IMAGE CREATOR]:[ERROR]: unable to open image file\n");
    }

    return image;
}