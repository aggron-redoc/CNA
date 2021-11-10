#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>

void main(int argc, char **argv)
{//ip address of server
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr;
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(8090);
  servaddr.sin_addr.s_addr=inet_addr(argv[1]);
  connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  printf("Filename: %s\n",argv[2]);
  FILE *p=fopen(argv[2],"wb");
  int si=1024;
  while(si>0)
  {
    recv(sockfd,(void *)&si,sizeof(int),0);
    printf("%d\n",si);
    if(si>0)
    {
      char buffer[si];
      recv(sockfd,buffer,si,0);
      fwrite(buffer,1,si,p);
    }
  }
  fclose(p);
  close(sockfd);
}
