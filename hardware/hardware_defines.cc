#include "hardware_aodv.h"

void printPacket(FILE* file, char * buffer, int length){
    for(int i = 0; i < length; i++){
        fprintf(file, "%c", buffer[i]);
    }
}
