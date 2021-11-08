#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

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

	printf("\nEnter the string :");
	scanf(" %[^\n]",str);
	len=sizeof(servaddr);
	sendto(sockfd,str,sizeof(str),0,(struct sockaddr*)&servaddr,len);
	recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&servaddr,&len);
	printf("\nThe message echoed : %s\n\n",buff);
	close(sockfd);
	return 0;
}
