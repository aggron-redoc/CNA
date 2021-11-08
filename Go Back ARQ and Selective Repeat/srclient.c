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

int main(int argc,char **argv)
{
  srand(time(NULL));
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in servaddr;
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(8000);
  servaddr.sin_addr.s_addr=inet_addr(argv[1]);
  connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
  int length;
  printf("Give the length: ");
  scanf("%d",&length);
  int *data=(int *)malloc(length*sizeof(int));
  printf("Give the data: ");
  for(int i=0;i<length;i++)
    scanf("%d",&data[i]);
  send(sockfd,(void *)&length,sizeof(int),0);
  int i=0;
  int flag_nak=0;
  while(i<length)
  {
    send(sockfd,(void *)&flag_nak,sizeof(int),0);
    send(sockfd,(void *)&i,sizeof(int),0);
    if(flag_nak==0)
    {
      int *window=(data + i);
      int flag=rand()%19;
      int tbca=-1;
      int tempval=-1;
      if(flag%2==1 && i+SLIDING_WINDOW-1<length)
      {
          tbca=rand()%SLIDING_WINDOW;
          tempval=window[tbca];
          window[tbca]=-1;
      }
      else if(flag%2==1)
      {
        tbca=rand()%(length-i);
        tempval=window[tbca];
        window[tbca]=-1;
      }
      if(i+SLIDING_WINDOW-1<length)
        send(sockfd,(void *)(window),sizeof(int)*SLIDING_WINDOW,0);
      else
        send(sockfd,(void *)(window),sizeof(int)*(length-i),0);
      char buffer[50];
      recv(sockfd,buffer,sizeof(buffer),0);
      if(strcmp(buffer,""))
      {
        printf("%s\n",buffer);
        if(!strncmp(buffer,"NAK",3))
        {
          i--;
          flag_nak=1;
        }
        i++;
      }
      else
      {
        printf("ACK lost! Resending lost ACK alone\n");
        flag_nak=1;
      }
      if(tbca!=-1)
        window[tbca]=tempval;
    }
    else
    {
      flag_nak=0;
      send(sockfd,(void *)&data[i],sizeof(int),0);
      char x[50];
      recv(sockfd,x,sizeof(x),0);
      printf("%s\n",x);
    }
  }
  free(data);
  close(sockfd);
}
