#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>


int main(int argc, char **argv)
{
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr;
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=inet_addr(argv[1]);
  servaddr.sin_port=htons(8080);
  connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  char filename[50];
  recv(sockfd,filename,sizeof(filename),0);
  FILE *tbc = fopen(filename,"w");
  char buffer[1024];
  while(strcmp("File sent completely",buffer))//strcmp returns 0 if equal, 1 if former is bigger, -1 if former is smaller
  {
    recv(sockfd,buffer,sizeof(buffer),0);
    if(strcmp("File sent completely",buffer))
    {
      fprintf(tbc,"%s\n",buffer);
    }
  }
  fclose(tbc);
  close(sockfd);
}
