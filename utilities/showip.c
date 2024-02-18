#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if(2 != argc)
	{
		fprintf(stderr,"usage: ./showip hostname\n");
		return 1;
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;	//can be either IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;

	if(0 != (status = getaddrinfo(argv[1], NULL, &hints, &res)))
	{
		fprintf(stderr,"[%s:%d] getaddrinfo: %s\n",__func__, __LINE__, gai_strerror(status));
		return 1;
	}

	printf("IP addresses for %s\n\n",argv[1]);

	for(p = res; p != NULL; p = p->ai_next)
	{
		void *addr;
		char *ipver;

		if(AF_INET == p->ai_family)
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in*) p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*) p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		//convert the IP to string
		inet_ntop(p->ai_family, addr, ipstr, INET6_ADDRSTRLEN);
		printf("%s: %s\n",ipver, ipstr);
	}

	freeaddrinfo(res);
	return 0;
}
