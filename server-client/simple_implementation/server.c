#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT		12345
#define IP_ADDRESS	"127.0.0.1"
#define MAX_BUFF 	1024 //bytes
#define BACKLOG		10	//number of connections allowed in the incoming queue.

void chat(int sockfd)
{
	char buff[MAX_BUFF];
	int n;

	while(1)
	{
		memset(buff, 0, sizeof(buff));

		if(-1 == recv(sockfd, buff, MAX_BUFF - 1,0))
		{
			fprintf(stderr,"[%s:%d] error in receiving message: %s\n",__func__, __LINE__, strerror(errno));
			exit(0);
		}
		else
			fprintf(stderr,"client>> %s",buff);

		memset(buff, 0, sizeof(buff));

		n = 0;

		fprintf(stderr,"server>> ");
		
		while((buff[n++] = getchar()) != '\n');

		if(-1 == (send(sockfd, buff, n, 0)))
		{
			fprintf(stderr,"[%s:%d] error in sedning the message: %s",__func__,__LINE__, strerror(errno));
			exit(0);
		}


		if(strncmp("exit", buff,4) == 0)
		{
			fprintf(stderr,"\nserver>> CLOSING!!\n");
			break;
		}

	}
}

int main()
{

	//struct addrinfo hints, *res;	//variables related to getaddrinfo()
	struct sockaddr_in server, client;
	int status;
	int s;	//socket descriptor
	char ip[INET_ADDRSTRLEN];

#if 0
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;		//IPv4
	hints.ai_socktype = SOCK_STREAM;	//TCP

//getaddrinfo() is making the code complicated will be revisited and implemented properly in the future

	if(0 != (status = getaddrinfo("127.0.0.1", "12345", &hints, &res)))
	{
		fprintf(stderr,"[%s:%d] getaddrinfo error: %s\n", __func__, __LINE__, gai_strerror(status));
		return 1;
	}
#endif

	if(-1 == (s = socket(AF_INET, SOCK_STREAM/*tcp*/, 0)))
	{
		fprintf(stderr,"[%s:%d] socket creation failed\n",__func__, __LINE__);
		exit(0);
	}
	else
		fprintf(stderr,"socket created successfully\n");
	
	server.sin_family = AF_INET;			//IPv4
	inet_pton(AF_INET, IP_ADDRESS, &(server.sin_addr));
	//server.sin_addr.s_addr = htonl(IP_ADDRESS);	//Assign loopback address
	server.sin_port = htons(PORT);				//Assign port 12345

	if(0 != (status = bind(s, (struct sockaddr *)&server, sizeof(server))))
	{
		fprintf(stderr,"[%s:%d] scoket bind failure error: %s\n", __func__, __LINE__, strerror(errno));
		exit(0);
	}
	else
	{
		inet_ntop(AF_INET, &(server.sin_addr), ip, INET_ADDRSTRLEN);
		fprintf(stderr,"socket bind successfully with [%s : %hu]\n", ip, server.sin_port);
	}
	
	if(0 != listen(s, BACKLOG))
	{
		fprintf(stderr,"[%s:%d] listen failure error: %s\n",__func__, __LINE__, strerror(errno));
		exit(0);
	}
	else
		fprintf(stderr,"server listening.....\n");
	
	unsigned int len = sizeof(client);

	int connfd;

	if(0 > (connfd = accept(s, (struct sockaddr*)&client, &len)))
	{
		fprintf(stderr,"[%s:%d] server accept failure error: %s\n",__func__, __LINE__, strerror(errno));
		exit(0);
	}
	else
		fprintf(stderr,"server accepted the client\n");

	chat(connfd);

	close(connfd);
	
	return 0;
}



