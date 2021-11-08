#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

using namespace std;

int main(int argc, char **argv)
{
  srand(time(NULL));
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr;
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=inet_addr(argv[1]);
  servaddr.sin_port=htons(8000);
  connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  int length;
  printf("Give the element: ");
  scanf("%d",&length);
  int count=0,c=0;int l=length;
  while(l>0)
  {
    count+=l%2;
    c++;
    l/=2;
  }
  int *p=(int *)malloc(sizeof(int)*2);
  p[0]=count%2;
  p[1]=length;
  printf("%d %d",p[0],p[1]);
  if((rand()%19)%2==1)
    p[1]=rand()%255;
  send(sockfd,(void *)p,sizeof(int)*2,0);
  char buffer[1024];
  memset(buffer,0,sizeof(buffer));
  recv(sockfd,buffer,sizeof(buffer),0);
  printf("\n%s\n",buffer);
  free(p);
  close(sockfd);
}
