/**
 *  \file fileReader.hpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 */
  #ifndef FILE_READER_H
    #define FILE_READER_H
    
    /*libraries*/
        #include <iostream>
        #include <cstring>
        #include <fstream>

        #include "Messages.h"
    /*end of libraries*/

    unsigned char* readFile(std::string filePath, uint64_t* length);
    void writeFile(std::string filePath, unsigned char* content, uint64_t content_len);

#endif