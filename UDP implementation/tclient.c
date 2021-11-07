#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argc,char **argv)
{
	socklen_t len;
	int sockfd,n;
	struct sockaddr_in servaddr,cliaddr;
	char buff[1024],str[1024];

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
		perror("\nsocket error\n");

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port=htons(5000);
	while(strcmp(str,"Exit!"))
	{
		strcpy(str,"");

		len=sizeof(servaddr);
		sendto(sockfd,str,sizeof(str),0,(struct sockaddr*)&servaddr,len);
		recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&servaddr,&len);
		printf("\nThe message echoed : %d\n\n",ntohs(servaddr.sin_port));
		recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&servaddr,&len);
		printf("\nThe message echoed : %d\n\n",ntohs(servaddr.sin_port));
	}
	close(sockfd);
	return 0;
}
