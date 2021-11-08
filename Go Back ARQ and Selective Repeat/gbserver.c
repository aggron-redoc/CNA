#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>
#define SLIDING_WINDOW 4

int main()
{
  srand(time(NULL));
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr,cliaddr;
  memset(&servaddr,0,sizeof(servaddr));
  memset(&cliaddr,0,sizeof(cliaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(8000);
  servaddr.sin_addr.s_addr=INADDR_ANY;
  bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  listen(sockfd,5);
  while(1)
  {
    socklen_t len=sizeof(cliaddr);
    int actives=accept(sockfd,(struct sockaddr *)&cliaddr,&len);
    int length=0;
    recv(actives,(void *)&length,sizeof(int),0);
    int *tdata=(int *)malloc(sizeof(int)*length);
    int *window=(int *)malloc(sizeof(int)*SLIDING_WINDOW);
    int i=0;
    while(i<length)
    {
        int temp;
        recv(actives,(void *)&temp,sizeof(int),0);
        i=temp;
        recv(actives,(void *)window,sizeof(int)*SLIDING_WINDOW,0);
        for(int j=0;j+i<length && j<SLIDING_WINDOW;j++)
        {
          tdata[i+j]=window[j];
          printf("%d ",tdata[i+j]);
        }
        printf("\n");
        if(tdata[i]!=-1)
        {
          if((rand()%19)%2==0)
          {
            char x[50];
            sprintf(x,"ACK %d",i);
            send(actives,x,sizeof(x),0);
          }
          else
            send(actives,"",sizeof(""),0);
        }
        else
        {
          send(actives,"",sizeof(""),0);
        }
    }
    free(window);
    free(tdata);
  }
}
