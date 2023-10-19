/**
 *  \file fileReader.cpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 */

#include "fileHandler.hpp"
using namespace std;

unsigned char* readFile(string filePath, uint64_t *length){
  /*
    CITATION: https://www.cplusplus.com/reference/istream/istream/read/
   */
   std::ifstream is (filePath, std::ifstream::binary);
  if (is) {
    // get length of file:
    is.seekg (0, is.end);
    *length = (uint64_t)is.tellg() + (uint64_t)filePath.length() + (uint64_t)1;
    is.seekg (0, is.beg);

    char * buffer = new char [*length];
    
    memcpy(buffer, filePath.c_str(), filePath.length() + 1);

    // read data as a block:
    is.read (buffer + filePath.length() + 1,*length - filePath.length() - 1);

    if (!is){
      ERROR_PRINT("Error while reading file, only %ld could be read", is.gcount());
    }
    
    is.close();
    return (unsigned char *)buffer;

  }
  
  ERROR_PRINT("Problem with opening file");
  
  /*END OF CITATION*/
  return nullptr;
}

void writeFile(string filePath, unsigned char* content, uint64_t content_len){
    ofstream file(filePath, std::ifstream::binary);

    if(!file.is_open()){
        ERROR_PRINT("File: %s %s", filePath.c_str(), "cannot be open");
    }

    for(uint64_t i = 0; i < content_len; i++){
      file << content[i];
    }


    file.close();
}