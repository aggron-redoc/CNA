#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<time.h>

using namespace std;

int main(int argc, char **argv)
{
  srand(time(NULL));
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr;
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=inet_addr(argv[1]);
  servaddr.sin_port=htons(5000);
  connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  int length;
  printf("Give no. of elements: ");
  scanf("%d",&length);
  int l=length+1;
  send(sockfd,(void *)&l,sizeof(l),0);
  int *data=(int *)(malloc(sizeof(int)*length));
  printf("Give the data: ");
  for(int i=0;i<length;i++)
    scanf("%d",&data[i]);
  for(int i=0;i<length;i++)
    printf("%d",data[i]);
  vector<bitset<8>> values;
  for(int i=0;i<length;i++)
  {
    values.push_back(bitset<8>(data[i]));
  }
  vector<int> count(8,0);
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<length;j++)
      count[i]+=(values[j][i]==1)?(1):(0);
  }
  int result=0;
  for(int i=0;i<8;i++)
  {
      result=result|(count[i]%2)<<(i);
  }
  int *finaldata=(int *)(malloc(sizeof(int)*l));
  finaldata[0]=result;
  for(int i=0;i<length;i++)
    finaldata[i+1]=data[i];
  printf("\n");
  for(int i=0;i<l;i++)
    printf("%d ",finaldata[i]);
  if((rand()%19)%2==1)
    finaldata[rand()%l]=rand()%255;
  send(sockfd,(void *)finaldata,sizeof(int)*l,0);
  char buffer[1024];
  recv(sockfd,buffer,sizeof(buffer),0);
  printf("\n%s\n",buffer);
  free(data);
}
