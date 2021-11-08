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
  int porter;
  sscanf(argv[1],"%d",&porter);
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr,cliaddr;
  memset(&servaddr,0,sizeof(servaddr));
  memset(&cliaddr,0,sizeof(cliaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(porter);
  servaddr.sin_addr.s_addr=INADDR_ANY;
  bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  socklen_t len=sizeof(cliaddr);
  listen(sockfd,5);
  int actives=accept(sockfd,(struct sockaddr *)&cliaddr,&len);
  char x[50];
  sprintf(x,"Hello from %s",argv[2]);
  send(actives,x,sizeof(x),0);
  close(sockfd);
  close(actives);
}
