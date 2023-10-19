/**
 *  \file packetSender.cpp
 *  \date 29.09.2021
 *  \author Pavel Sestak
 */

#include "packetSender.hpp"

using namespace std;

int layer_2_header_length = 0;

uint64_t content_len = 0;
unsigned char *content;
std::string fileName;
uint64_t messageSize = 0;
uint32_t icmp_id_message=0;
unsigned char *aesIv;
int family = 0;
uint64_t PROTOCOL_IDENTIFIER = 0xA7E0C13E9815EDF0;
uint64_t protocol_identifier_message = 0;
uint32_t source_address;
time_t time_of_last_packet;

void clearSniffingData(){
	free(content);
	free(aesIv);
	content = nullptr;
	content_len = 0;
	fileName = "";
	icmp_id_message = 0;
	protocol_identifier_message = 0;
	source_address = 0;
	time_of_last_packet = 0;
}

void processPacket(__attribute__((unused)) u_char *args,const struct pcap_pkthdr *header, const u_char *buffer)
{	
	struct iphdr * iphdr =(struct iphdr*)(buffer + layer_2_header_length);

	int ipHeaderLength = sizeof(struct iphdr); //20bytes

	DEBUG_PRINT("IP VERSION: %d", iphdr->version);
	DEBUG_PRINT("PACKET OFFSET: %d", layer_2_header_length+ipHeaderLength);
	u_char packet[PING_PKT_S];
	char* message;
	int id = 0;
	uint16_t icmp_sequence = 0;
	
	struct icmp_pkt pkt;
	memcpy(&pkt, buffer + layer_2_header_length+ipHeaderLength, sizeof(struct icmp_pkt));

	if(iphdr->version == 6){
		ipHeaderLength = 40; //IPV6 has different header size
		memcpy(&packet, buffer + ipHeaderLength + layer_2_header_length, sizeof(struct icmp6hdr) + PKT_DATA_LEN);

		struct icmpv6_pkt *v6packet = (struct icmpv6_pkt *)packet;
		message = v6packet->msg;
		id = v6packet->hdr.identifier;
		icmp_sequence = v6packet->hdr.sequence;
	}
	else{
		memcpy(&packet, buffer + ipHeaderLength + layer_2_header_length, sizeof(struct icmphdr) + PKT_DATA_LEN);
		struct icmp_pkt *v4packet = (struct icmp_pkt *)packet;
		message = v4packet->msg;
		id = v4packet->hdr.un.echo.id;
		icmp_sequence = v4packet->hdr.un.echo.sequence;
	}

	DEBUG_PRINT("icmp id: %d", id);
	DEBUG_PRINT("icmp seq: %d", icmp_sequence);

	int data_offset = 0;

	if (content_len == 0)
	{
		source_address = iphdr->saddr;

		memcpy(&protocol_identifier_message, message, sizeof(uint64_t));
		
		if(protocol_identifier_message != PROTOCOL_IDENTIFIER){
			DEBUG_PRINT("WRONG PROTOCOL IDENTIFIER: %ld", protocol_identifier_message);
			return;
		}

		memcpy(&messageSize, message + 8, sizeof(uint64_t));
		aesIv = (unsigned char *)malloc(17);
		if (aesIv == nullptr)
		{
			ERROR_PRINT("Intern error, error while allocating memory");
		}

		memcpy(aesIv, message + 16, 16);
		memcpy(aesIv+16, "\0", 1); //add null terminate symbol

		data_offset = 32;

		icmp_id_message = id;
	}

	/*Check if its our ICMP packet*/
	if(id != icmp_id_message || source_address != iphdr->saddr){
		DEBUG_PRINT("Different ICMP ID");

		if(time_of_last_packet == 0){
			time_of_last_packet = header->ts.tv_sec;
		}

		//Time from last succesfully read packet is bigger than 15 secs, predict user interupt sending
		if( (header->ts.tv_sec - time_of_last_packet) > 15 ){
			clearSniffingData();
		}
		return;
	}


	time_of_last_packet = header->ts.tv_sec;

	DEBUG_PRINT("Message size: %ld", messageSize);

	uint64_t dataSize = messageSize - content_len;
	if ((dataSize + data_offset) >= PKT_DATA_LEN)
	{
		dataSize = PKT_DATA_LEN;
	}

	if (content == nullptr)
	{
		content = (unsigned char *)realloc(content, messageSize);
		if (content == nullptr)
		{
			ERROR_PRINT("Intern error, with allocating memory");
		}
	}

	DEBUG_PRINT("ContentLen: %d, dataOffset: %d, dataSize: %ld", content_len, data_offset, dataSize);

	memcpy(content + content_len, message + data_offset, dataSize);

	content_len += dataSize - data_offset;

	DEBUG_PRINT("New content_len %d, datasize: %ld", content_len, dataSize);

	//File is fully read
	if (content_len >= messageSize)
	{
		
		unsigned char *decryptedtext = (unsigned char *)malloc(content_len);
		if (decryptedtext == NULL)
		{
			ERROR_PRINT("Intern error: unable to alocate memory");
		}

		DEBUG_PRINT("ContentLen: %d, AesKey: %s, aesIv: %s, decryptedText: %d",(int)content_len, AesKey, aesIv, decryptedtext);

		int decryptedtext_len = AesEncrypter::aes_decrypt(content, (int)content_len, AesKey, aesIv, decryptedtext);

		data_offset = 0;
		while (decryptedtext[data_offset] != 0)
		{
			fileName += decryptedtext[data_offset++];
		}
		data_offset++;
		
		DEBUG_PRINT("Decrypted filename: %s", fileName.c_str());
		DEBUG_PRINT("Decrypted content length: %d", decryptedtext_len);

		writeFile(fileName, decryptedtext + data_offset, decryptedtext_len - data_offset);
		free(decryptedtext);
		clearSniffingData();
	}
}

pcap_t *openConnection()
{
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	handle = pcap_open_live(INTERFACE, BUFSIZ, 1, 1000, errbuf);

	if (handle == nullptr)
	{
		ERROR_PRINT("Error while opening interface");
	}

	//set filtering to icmp echo only
	const char* filter = "(icmp or icmp6) and( \
	icmp[icmptype] == icmp-echo or icmp6[icmp6type] == icmp6-echo) ";

	bpf_program fp;

	if (pcap_compile(handle, &fp, filter, 0, PCAP_NETMASK_UNKNOWN) == PCAP_ERROR)
	{
		ERROR_PRINT("Error calling pcap_compile");
	}

	/* set the compiled program as the filter */
	if (pcap_setfilter(handle, &fp) == PCAP_ERROR)
	{
		ERROR_PRINT("Error setting filter");
	}

	if (handle == NULL)
	{
		ERROR_PRINT("Coudn't open device: %s", errbuf);
	}

	switch (pcap_datalink(handle))
	{
	case DLT_EN10MB: // Ethernet header
		layer_2_header_length = 14;
		break;

	case DLT_LINUX_SLL: // Linux cooked header
		layer_2_header_length = 16;
		break;
	}

	return handle;
}

int read_content_icmp(unsigned char *content)
{
	pcap_t *handle;

	int content_len = 0;

	content = (unsigned char *)malloc(1); //alloc one char
	if (content == nullptr)
	{
		ERROR_PRINT("Error while allocating memory");
	}

	handle = openConnection();

	pcap_loop(handle, 0, (pcap_handler)processPacket, nullptr);
	
	return content_len;
}

void send_content_icmp(unsigned char *content, uint64_t content_len, int sockfd, unsigned char *aesIv)
{
	uint16_t msg_count = 0;
	struct icmp_pkt pckt;

	bzero(&pckt, sizeof(pckt));

	uint64_t contentIterator = 0;
	uint64_t prevContentIterator = 0;

	memcpy(pckt.msg, &PROTOCOL_IDENTIFIER, 8);
	memcpy(pckt.msg + 8, &content_len, 8);
	memcpy(pckt.msg + 16, aesIv, 16);

	uint64_t i = 32;

	DEBUG_PRINT("protocol identifier: %lu", PROTOCOL_IDENTIFIER);
	DEBUG_PRINT("ContentLength: %ld", content_len);
	DEBUG_PRINT("aes IV: %s", aesIv);
	
	struct timeval tv_out;
    tv_out.tv_sec = 4;
    tv_out.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
                   (const char*)&tv_out, sizeof tv_out);

	while (contentIterator < content_len)
	{
		prevContentIterator = contentIterator;
		DEBUG_PRINT("CONTENT ITERATOR: %ld", contentIterator);
		if (contentIterator != 0)
		{
			//filling packet
			bzero(&pckt, sizeof(pckt));
		}

		pckt.hdr.type = 128;	  //Set header type to ICMP_ECHO V6
		if(family == AF_INET){
			pckt.hdr.type = ICMP_ECHO;	  //Set header type to ICMP_ECHO
		}

		for (; i < sizeof(pckt.msg); i++)
		{
			pckt.msg[i] = content[contentIterator];
			contentIterator++;
			if (contentIterator == content_len)
			{
				break;
			}
		}

		i = 0;

		pckt.hdr.un.echo.sequence = msg_count++;

		if((send(sockfd, &pckt, sizeof(pckt), 0)) == -1){
			ERROR_PRINT("Packet Sending Failed!");
		}

 		
		struct sockaddr r_addr;
		socklen_t addr_len=sizeof(r_addr);
		struct icmp_pkt inBuffer;

        if (recvfrom(sockfd, &inBuffer, sizeof(inBuffer), 0, 
             &r_addr, &addr_len) <= 0
              && msg_count>1) 
        {
			contentIterator = prevContentIterator;

			DEBUG_PRINT("PACKET RECEIVE FAIL, ITERATOR AFTER SUB: %ld", contentIterator);
        }
		else{
			//DEBUG_PRINT("SUCCESS");
		}
	}
}

int getSocketFd(addrinfo *result)
{
	int sockfd;
	addrinfo *rp;
	
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{	
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1)
			continue;

		if(connect(sockfd, rp->ai_addr, rp->ai_addrlen) == -1){
			close(sockfd);
			continue;
		}

		break; /* Success */
	}

	if (rp == NULL)
	{ /* No address succeeded */
		ERROR_PRINT("Could not bind socket (maybe you forgot to run as root)");
	}

	family = rp->ai_family;	
	return sockfd;
}