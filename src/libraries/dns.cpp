/**
 *  \file dns.cpp
 *  \date 28.09.2021
 *  \author Pavel Sestak
 */

#include "dns.hpp"

// Performs a DNS lookup 
struct addrinfo * dns_lookup(char *addr_host)
{
    struct addrinfo hints;
	struct addrinfo *result;
	int s;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	/* Allow IPv4 or IPv6 */
	//hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags = 0;	/* For wildcard IP address */
	hints.ai_protocol = IPPROTO_ICMP;

	s = getaddrinfo(addr_host, NULL, &hints, &result);
	if (s != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	if(result->ai_family == AF_INET6){
		result->ai_protocol = IPPROTO_ICMPV6;
	}

	result->ai_socktype = SOCK_DGRAM;
    return result;
}