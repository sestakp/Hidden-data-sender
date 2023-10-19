/**
 *  \file packetSender.hpp
 *  \date 29.09.2021
 *  \author Pavel Sestak
 */
  #ifndef PACKET_SENDER_H
    #define PACKET_SENDER_H
    
    /*libraries*/
        #include <iostream>
        #include <stdio.h>
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <unistd.h>
        #include <string.h>
        #include <stdlib.h>
        #include <netinet/ip_icmp.h>
        #include <time.h>
        #include <fcntl.h>
        #include <signal.h>
        #include <time.h>
        #include <string>

        #include "Messages.h"
        #include "dns.hpp"
		#include "aes.hpp"
        #include "fileHandler.hpp"
        
        #include <pcap.h> //Library for packet sniffing www.tcpdump.org/pcap.html
    /*end of libraries*/

    // Define the Packet Constants
    // ping packet size
    #define PING_PKT_S 1500
    #define PKT_DATA_LEN 1000

    //Define interface to sniffing
    #define INTERFACE "any"
    

    // ping packet structure
    struct icmp_pkt
    {
        struct icmphdr hdr;
        char msg[PKT_DATA_LEN];
    };

    struct icmp6hdr{
        uint8_t type;
        uint8_t code;
        uint16_t checksum;
        uint16_t identifier;
        uint16_t sequence;
    };

    struct icmpv6_pkt
    {
        struct icmp6hdr hdr;
        char msg[PKT_DATA_LEN];
    };

    int getSocketFd(addrinfo *result);
    void intHandler(int dummy);
    void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr, char *ping_dom, char *ping_ip, char *rev_host);
    void send_content_icmp(unsigned char *content, uint64_t content_len, int sockfd, unsigned char* aesIv);
    int read_content_icmp(unsigned char *content);

#endif
