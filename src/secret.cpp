/**
 *  \file secret.cpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 */

#include "secret.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    argumentParse(argc, argv, &filePath, &destination, &listenFlag);
    int ciphertext_len;
    unsigned char *ciphertext = nullptr;
    uint64_t plainText_len;
    unsigned char *plainText;
    struct addrinfo *result;

    
    if (listenFlag)
    {
        ciphertext_len = read_content_icmp(ciphertext);
    }
    else
    {
        result = dns_lookup((char *)destination.c_str());
        int sockfd = getSocketFd(result);
	    freeaddrinfo(result);
        
        plainText = readFile(filePath, &plainText_len);

        uint64_t aes_padding = (AES_BLOCK_SIZE - (plainText_len % AES_BLOCK_SIZE));
        ciphertext = (unsigned char *)malloc(plainText_len + aes_padding);
        if (ciphertext == NULL)
        {
            ERROR_PRINT("Intern error: unable to alocate memory");
        }

        std::string iv = AesEncrypter::generateInitialVector();
        unsigned char *aesIv = (unsigned char *)iv.c_str();

        ciphertext_len = AesEncrypter::aes_encrypt(plainText, (int)plainText_len, AesKey, aesIv, ciphertext);

        send_content_icmp(ciphertext, ciphertext_len, sockfd, aesIv);

        delete []plainText;
        free(ciphertext);
    }
    return EXIT_SUCCESS;
}