#include<bits/stdc++.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<pthread.h>


using namespace std;
unordered_map<string,sockaddr_in> DNS;
void *Input_more_pairs(void *arg)
{
  sockaddr_in pl;
  pl.sin_family=AF_INET;
  pl.sin_port=htons(7631);
  pl.sin_addr.s_addr=inet_addr("127.0.0.1");
  while(1)
  {
    string domainname;
    printf("Give string: ");
    cin>>domainname;
    int porter;
    printf("Give portnumber: ");cin>>porter;
    if(porter!=8080){
      pl.sin_port=htons(porter);
      DNS[domainname]=pl;
    }
    else
    {
      printf("Please try again!!\n");
    }
  }
}


int main()
{
  pthread_t l;
  pthread_create(&l,NULL,Input_more_pairs,NULL);
  struct sockaddr_in servaddr,cliaddr;
  int sockfd=socket(AF_INET,SOCK_DGRAM,0);
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(8080);
  servaddr.sin_addr.s_addr=INADDR_ANY;
  bind(sockfd,(sockaddr *)&servaddr,sizeof(servaddr));
  memset(&cliaddr,0,sizeof(cliaddr));
  while(1)
  {
    char buffer[1024];
    socklen_t len=sizeof(cliaddr);
    recvfrom(sockfd,buffer,sizeof(buffer),0,(sockaddr *)&cliaddr,&len);
    printf("Received request from %d",ntohs(cliaddr.sin_port));
    string buf(buffer);
    if(DNS.find(buf)==DNS.end())
    {
      sendto(sockfd,"Corresponding Server not found!!",sizeof("Corresponding Server not found!!"),0,(sockaddr *)&cliaddr,len);
      continue;
    }
    sendto(sockfd,"Found the address!!",sizeof("Found the address!!"),0,(sockaddr *)&cliaddr,len);
    sendto(sockfd,(void *)&DNS[buf],sizeof(DNS[buf]),0,(sockaddr *)&cliaddr,len);
  }
}
