#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>

#define MAX_BUFF	1024
#define PORT		12345
#define IP_ADDRESS 	"127.0.0.1"

void chat(int sockfd)
{
	char buff[MAX_BUFF];
	int n;

	while(1)
	{
		memset(buff, 0, sizeof(buff));
		
		fprintf(stderr,"client>> ");
		n = 0;
		while((buff[n++] = getchar()) != '\n');

		if(-1 == send(sockfd, buff, n, 0))
		{
			fprintf(stderr,"[%s:%d] error in sending the message: %s",__func__,__LINE__, strerror(errno));
			exit(0);
		}
		
		memset(buff, 0, sizeof(buff));

		int status = recv(sockfd, buff, MAX_BUFF - 1, 0);

		if(-1 == status)
		{
			fprintf(stderr,"[%s:%d] error in receiving the message: %s",__func__, __LINE__, strerror(errno));
			exit(0);
		}
		else
			fprintf(stderr,"server>> %s",buff);
		if(0 == strncmp("exit",buff, 4))
		{
			break;
		}
	}

}

int main()
{
	int s, connfd;
	struct sockaddr_in server;

	if(-1 == (s = socket(AF_INET, SOCK_STREAM, 0)))
	{
		fprintf(stderr,"[%s:%d] socket creation failed\n",__func__, __LINE__);
		exit(0);
	}
	else
		fprintf(stderr,"socket created successfully\n");
	
	server.sin_family = AF_INET;
	inet_pton(AF_INET, IP_ADDRESS, &(server.sin_addr));
	server.sin_port = htons(PORT);

	if(0 != connect(s, (struct sockaddr*)&server, sizeof(server)))
	{
		fprintf(stderr,"[%s:%d] connection with the server failed: %s\n",__func__,__LINE__,strerror(errno));
		exit(0);
	}
	else
		fprintf(stderr,"connection to server successful\n");
	
	chat(s);

	close(s);

	return 0;
}

