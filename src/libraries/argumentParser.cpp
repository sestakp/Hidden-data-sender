/**
 *  \file argumentParser.cpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 */
#include "argumentParser.hpp"

void argumentParse(int argc, char *argv[], std::string *filePath, std::string *destination, bool *listenFlag){
    bool argumentFile = false;
    bool argumentDestination = false;

    for(int i = 1; i < argc; i++){
        
        std::string arg = argv[i];

        if(arg.compare("-r") == 0){
            if((i+1) < argc){
                argumentFile = true;
                i++;
                *filePath = argv[i];
            }
            else{
                ERROR_PRINT("Argument -r without value")
            }
        }
        else if(arg.compare("-s") == 0){
            if((1+i) < argc){ 
                argumentDestination = true;
                i++;
                *destination = argv[i];
            }
            else{
                ERROR_PRINT("Argument -s without value");
            }
        }
        else if(arg.compare("-l") == 0){
            *listenFlag = true;
        }
        else{
            ERROR_PRINT("Unknown argument");
        }
    }

    if( ! ((argumentFile && argumentDestination) || *listenFlag)){
        ERROR_PRINT("Arguments -r -s are mandatory when listen flag not mentioned");
        std::cerr << "ERROR";
    }
}