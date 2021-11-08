#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char **argv)
{
	socklen_t len;
	int sockfd,n;
	struct sockaddr_in servaddr,cliaddr;
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
		len=sizeof(cliaddr);
		recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,&len);
		printf("\nMessage received from the cient is: %s\n",buff);
		sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,len);
	}
	close(sockfd);
	return 0;
}
