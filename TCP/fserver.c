#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>



void main()
{
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr,cliaddr;
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=INADDR_ANY;
  servaddr.sin_port=htons(8080);
  memset(&cliaddr,0,sizeof(cliaddr));
  bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  listen(sockfd,5);
  socklen_t len=sizeof(cliaddr);
  int new_socket=accept(sockfd,(struct sockaddr *)&cliaddr,&len);
  char filename[50];
  printf("Give the filename: ");
  scanf(" %[^\n]",filename);
  send(new_socket,filename,sizeof(filename),0);
  FILE *filer=fopen(filename,"r");
  char buffer[1024];
  while(fscanf(filer," %[^\n]",buffer)!=-1)
  {
    send(new_socket,buffer,sizeof(buffer),0);
  }
  send(new_socket,"File sent completely",sizeof("File sent completely"),0);
  fclose(filer);
  close(new_socket);
  close(sockfd);
}
