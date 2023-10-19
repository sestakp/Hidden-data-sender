/**
 *  \file secret.hpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 */
 /*Include guard*/
#ifndef SECRET_H
    #define SECRET_H
    
    /*Libraries*/
        //IO 
        #include <iostream>
        #include <cstring>

        //Encryption
        #include <openssl/conf.h>
        #include <openssl/evp.h>
        #include <openssl/err.h>
        #include <openssl/aes.h>
        #include <openssl/rand.h>

        //Modules
        #include "libraries/aes.hpp"
        #include "libraries/argumentParser.hpp"
        #include "libraries/fileHandler.hpp"
        #include "libraries/dns.hpp"
        #include "libraries/packetSender.hpp"
        #include "libraries/Messages.h"

    /*End of libraries*/

    /*Variables*/
        
        /*Arguments*/
            std::string filePath = "";

            std::string destination = "";

            bool listenFlag = false;
        /*End of arguments*/

    /*End of variables*/


    /*Constants*/

        /*Exit codes*/
            #define EXIT_FAILURE 1
            #define EXIT_SUCCESS 0
        /*End of exit codes*/
        
    /*End of constants*/

#endif