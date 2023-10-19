/**
 *  \file dns.hpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 */

 #ifndef DNS_H
    #define DNS_H

    /*libraries*/
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <string.h>
        #include <stdlib.h>
        #include "Messages.h"

        #include <sys/types.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>
       #include <sys/socket.h>
       #include <netdb.h>

    /*end of libraries*/

    struct addrinfo * dns_lookup(char *addr_host);
 #endif