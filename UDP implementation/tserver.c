#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char **argv)
{
	socklen_t len;
	int sockfd,n;int flag=0;
	struct sockaddr_in servaddr,cliaddr,cliaddr2;
	char buff[1024];

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
		perror("\nunable to create socket\n\n");

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(5000);

	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	for(;;)
	{
		if(flag==0)
		{
			len=sizeof(cliaddr);
			recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,&len);
			printf("\nMessage received from the client is: %s %d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
			sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,len);flag++;
		}
		else
		{
			len=sizeof(cliaddr2);
			recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr2,&len);
			printf("\nMessage received from the client is: %s %d\n",inet_ntoa(cliaddr2.sin_addr),ntohs(cliaddr2.sin_port));
			sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr2,len);
			sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,len);flag=0;
		}
	}
	close(sockfd);
	return 0;
}
