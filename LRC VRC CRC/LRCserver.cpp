#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<bits/stdc++.h>
#include<stdlib.h>

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
  servaddr.sin_port=htons(5000);
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=INADDR_ANY;
  if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
  {
    printf("Binding error");
    exit(0);
  }
  listen(sockfd,5);
  socklen_t x=sizeof(cliaddr);
  while(1)
  {
    int new_connection=accept(sockfd,(struct sockaddr *)&cliaddr,&x);
    int length;
    recv(new_connection,(void *)&length,sizeof(length),0);
    printf("Recieved value: %d",length);
    int *data=(int *)(malloc(sizeof(int)*length));
    recv(new_connection,(void *)data,sizeof(int)*length,0);
    printf("\nReceived value: ");
    for(int i=0;i<length;i++)
      printf("%d ",data[i]);
    printf("\n");
    vector<bitset<8>> values;
    for(int i=0;i<length;i++)
    {
      values.push_back(bitset<8>(data[i]));
    }
    vector<int> count(8,0);
    for(int i=0;i<8;i++)
    {
      for(int j=1;j<length;j++)
        count[i]+=(values[j][i]==1)?(1):(0);
    }
    int result=0;
    for(int i=0;i<8;i++)
    {
        result=result|(count[i]%2)<<(i);
    }
    if(result==data[0])
      send(new_connection,"Data came without any issue",sizeof("Data came without any issue"),0);
    else
      send(new_connection,"Erronous Packet ignored! Resend!",sizeof("Erronous Packet ignored! Resend!"),0);
    free(data);
  }
}
