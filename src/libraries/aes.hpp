/**
 *  \file aes.hpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 *  \see https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 */
 /*Include guard*/
#ifndef _AES_H_
    #define _AES_H_
    
    /*libraries*/

        //Encryption
        #include <openssl/conf.h>
        #include <openssl/evp.h>
        #include <openssl/err.h>
        #include <openssl/aes.h>
        #include <openssl/rand.h>

        #include <string>
        #include <iostream>
        #include <random>

        #include "Messages.h"

    /*end of libraries*/


    extern unsigned char* AesKey;

    class AesEncrypter
    {
    public:
        static int aes_encrypt(unsigned char *plaintext, 
                                             int plaintext_len, 
                                             unsigned char *key, 
                                             unsigned char *iv, 
                                             unsigned char *ciphertext);

        static int aes_decrypt(unsigned char *ciphertext, 
                              int ciphertext_len, unsigned char *key,
                              unsigned char *iv, 
                              unsigned char *plaintext);

        static std::string generateInitialVector();
    };
#endif