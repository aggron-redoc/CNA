#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>

int main(int argc,char **argv)
{
  int sockfd=socket(AF_INET,SOCK_DGRAM,0);
  struct sockaddr_in dns,servaddr;
  dns.sin_family=AF_INET;
  dns.sin_port=htons(8080);
  dns.sin_addr.s_addr=inet_addr("127.0.0.1");
  memset(&servaddr,0,sizeof(servaddr));
  sendto(sockfd,argv[1],sizeof(argv[1]),0,(struct sockaddr *)&dns,sizeof(dns));
  char buffer[1024];
  socklen_t len=sizeof(dns);
  recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&dns,&len);
  if(!strcmp(buffer,"Found the address!!"))
  {
    recvfrom(sockfd,(void *)&servaddr,sizeof(servaddr),0,(struct sockaddr *)&dns,&len);
    close(sockfd);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    recv(sockfd,buffer,sizeof(buffer),0);
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
  }
  close(sockfd);
}
