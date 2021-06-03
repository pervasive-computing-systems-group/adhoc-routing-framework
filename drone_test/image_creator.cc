#include "image_creator.h"

ImageCreator::ImageCreator(){

}

string ImageCreator::getData(){
    ifstream myfile ("image1.jpg", ios_base::binary);
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