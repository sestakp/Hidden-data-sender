/**
 *  \file argumentParser.hpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 */
 #ifndef ARGUMENT_PARSER_H
    #define ARGUMENT_PARSER_H
    
    /*libraries*/
        //IO 
        #include <iostream>
        #include <cstring>
        #include "Messages.h"

    /*end of libraries*/

    void argumentParse(int argc, char *argv[], std::string *filePath, std::string *destination, bool *listenFlag);
#endif