#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<unistd.h>

using namespace std;

int main()
{
  int sockfd;
  if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
  {
    printf("Error in socket creation!!\n");
    exit(0);
  }
  struct sockaddr_in servaddr,cliaddr;
  memset(&servaddr,0,sizeof(servaddr));
  memset(&cliaddr,0,sizeof(cliaddr));
  servaddr.sin_port=htons(8000);
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=INADDR_ANY;
  if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
  {
    printf("Binding error");
    exit(0);
  }
  int summa=listen(sockfd,5);
  socklen_t x=sizeof(cliaddr);
  while(1)
  {
    int new_connection=accept(sockfd,(struct sockaddr *)&cliaddr,&x);
    int *length=(int *)malloc(sizeof(int)*2);
    recv(new_connection,(void *)length,sizeof(length)*2,0);
    printf("Recieved value: %d %d",length[0],length[1]);
    int l=length[1],count=0;
    while(l>0)
    {
      count+=(l%2);
      l/=2;
    }
    printf("\nParity bit: %d",length[0]);
    if(count%2==length[0])
      send(new_connection,"Data came without any issue",sizeof("Data came without any issue"),0);
    else
      send(new_connection,"Erronous Packet ignored! Resend!",sizeof("Erronous Packet ignored! Resend!"),0);
    free(length);
  }
}
