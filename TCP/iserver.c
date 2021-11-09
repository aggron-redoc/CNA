#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>

void main()//for getting the filename
{
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr, cliaddr;
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(8090);
  servaddr.sin_addr.s_addr=INADDR_ANY;
  memset(&cliaddr,0,sizeof(cliaddr));
  socklen_t len=sizeof(cliaddr);
  bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  listen(sockfd,5);
  int new_socket=accept(sockfd,(struct sockaddr *)&cliaddr,&len);
  FILE *p = fopen("im3.jpeg","rb");
  send(new_socket,"im3.jpeg",sizeof("im3.jpeg"),0);
  char buffer[1024];
  int si;
  while((si=fread(buffer,1,sizeof(buffer),p))>0)
  {
    send(new_socket,(void *)&si,sizeof(int),0);
    printf("%d\n",si);
    send(new_socket,buffer,si,0);
    memset(buffer,0,sizeof(buffer));
  }
  printf("%d\n",si);
  send(new_socket,(void *)&si,sizeof(int),0);
  fclose(p);
  close(new_socket);
  close(sockfd);
}
