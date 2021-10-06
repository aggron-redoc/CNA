#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<pthread.h>
#define MAXLINE 1024

struct val_req_players
{
  struct sockaddr_in players[2];
  int porter;
};

int av_ports={{8090,1},{8092,1}};
int flag=0;
int len,n;
char buffer[MAXLINE];
int refree(char *ttt[])
{
  //1 for game in line
  //0 for draw
  //2 for player1 win
  //3 for player2 win
  int count=0;
  for(int i=0;i<3;i++)
    for(int j=0;j<strlen(ttt[i]);j++)
      if(ttt[i][j]=='X' || ttt[i][j]=='O')count++;
  if((ttt[0][0]=='X' && ttt[1][1]=='X' && ttt[2][2]=='X') ||
   (ttt[0][0]=='X' && ttt[0][1]=='X' && ttt[0][2]=='X') ||
   (ttt[1][0]=='X' && ttt[1][1]=='X' && ttt[1][2]=='X') ||
   (ttt[2][0]=='X' && ttt[2][1]=='X' && ttt[2][2]=='X') ||
   (ttt[0][0]=='X' && ttt[1][0]=='X' && ttt[2][0]=='X') ||
   (ttt[0][1]=='X' && ttt[1][1]=='X' && ttt[2][1]=='X') ||
   (ttt[0][2]=='X' && ttt[1][2]=='X' && ttt[2][2]=='X') ||
   (ttt[0][2]=='X' && ttt[1][1]=='X' && ttt[2][0]=='X'))
    return 2;
    else if (
             (ttt[0][0]=='O' && ttt[1][1]=='O' && ttt[2][2]=='O') ||
             (ttt[0][0]=='O' && ttt[0][1]=='O' && ttt[0][2]=='O') ||
             (ttt[1][0]=='O' && ttt[1][1]=='O' && ttt[1][2]=='O') ||
             (ttt[2][0]=='O' && ttt[2][1]=='O' && ttt[2][2]=='O') ||
             (ttt[0][0]=='O' && ttt[1][0]=='O' && ttt[2][0]=='O') ||
             (ttt[0][1]=='O' && ttt[1][1]=='O' && ttt[2][1]=='O') ||
             (ttt[0][2]=='O' && ttt[1][2]=='O' && ttt[2][2]=='O') ||
             (ttt[0][2]=='O' && ttt[1][1]=='O' && ttt[2][0]=='O'))
    return 3;
    else if(count!=9)
      return 1;
    else return 0;
}

void* tictactoe(void *arg)
{
  char *x[]={"___", "___", "___"};
  int gamesock;
  struct sockaddr_in servaddr;
  if ( (gamesock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
  {
       perror("socket creation failed");
       exit(EXIT_FAILURE);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  struct sockaddr_in player1=arg->players[0];
  struct sockaddr_in player2=arg->players[1];
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(arg->porter);
  if ( bind(gamesock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  int i=0;int m,u;int result=1;
  while((result=refree(x))==1)
  {
    if((i++)%2==0)
    {
      n=recvfrom(gamesock,(char *)buffer,MAXLINE,MSG_WAITALL,(struct sockaddr *) &player1, &len);
      buffer[n]='\0';
      sscanf(buffer,"%d %d",&m,&n);
      x[m][u]='X';
    }
    else
    {
      n=recvfrom(gamesock,(char *)buffer,MAXLINE,MSG_WAITALL,(struct sockaddr *) &player2, &len);
      buffer[n]='\0';
      sscanf(buffer,"%d %d",&m,&n);
      x[m][u]='O';
    }
  }
  if(result==2)
  {
    sendto(sockfd,(const char *)"X won",strlen("X won"),MSG_CONFIRM,(const struct *)&player1,len);
    sendto(sockfd,(const char *)"X won",strlen("X won"),MSG_CONFIRM,(const struct *)&player2,len);
  }
  else if(result==3)
  {
    sendto(sockfd,(const char *)"O won",strlen("X won"),MSG_CONFIRM,(const struct *)&player1,len);
    sendto(sockfd,(const char *)"O won",strlen("X won"),MSG_CONFIRM,(const struct *)&player2,len);
  }
  else if(result==0)
  {
    sendto(sockfd,(const char *)"Draw",strlen("X won"),MSG_CONFIRM,(const struct *)&player1,len);
    sendto(sockfd,(const char *)"Draw",strlen("X won"),MSG_CONFIRM,(const struct *)&player2,len);
  }
  if(av_ports[0][0]==porter)
    av_ports[0][1]=1;
  else if(av_ports[1][0]==porter)
    av_ports[1][1]=1;
  flag=0;
}

void main()
{
  int sockfd;
  //tic tac toe
  struct sockaddr_in servaddr, cliaddr,cliaddr2;
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));
  memset(&cliaddr2, 0, sizeof(cliaddr2));
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
  {
       perror("socket creation failed");
       exit(EXIT_FAILURE);
  }
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(8080);
  if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  int ipandport[]={0};
  while(1)
  {
    if(flag==0)
    {
      int len,n;
      n=recvfrom(sockfd,(char *)buffer,MAXLINE,MSG_WAITALL,(struct sockaddr *) &cliaddr, &len);
      buffer[n]='\0';
      if(!(strcmp("match",buffer)))
      {
        for(int i=0;i<2;i++)
          if(av_ports[i][1]==1)
          {
            flag=1;av_ports[1]=2;
            i+=2;
          }
      }
      sendto(sockfd,(const char *)"Waiting for your opponent to join",strlen("Waiting for your opponent to join"),MSG_CONFIRM,(const struct *)&cliaddr,len);
    }
    else if(flag==1)
    {
      n=recvfrom(sockfd,(char *)buffer,MAXLINE,MSG_WAITALL,(struct sockaddr *) &cliaddr2, &len);
      buffer[n]='\0';
      struct val_req_players v;
      if(!(strcmp("match",buffer)))
      {
        for(int i=0;i<2;i++)
          if(av_ports[i][1]==2)
          {
            flag=0;av_ports[1]=0;
            i+=2;
            v.porter=av_ports[i][0];
          }

        sendto(sockfd,(const char *)"Opponent found! Initiating the match!",strlen("Opponent found! Initiating the match!"),MSG_CONFIRM,(const struct *)&cliaddr,len);
        sendto(sockfd,(const char *)"Opponent found! Initiating the match!",strlen("Opponent found! Initiating the match!"),MSG_CONFIRM,(const struct *)&cliaddr2,len);
        v.players[0]=cliaddr;
        v.players[1]=cliaddr2;
        pthred_t x;
        if(av_ports[0][1]==0 && av_ports[1][1]==0)flag=-1;
        pthread_create(&x, NULL, &tictactoe, (void *)(&v));
      }
      memset(&cliaddr, 0, sizeof(cliaddr));
      memset(&cliaddr2, 0, sizeof(cliaddr2));
    }
    else
    {
      n=recvfrom(sockfd,(char *)buffer,MAXLINE,MSG_WAITALL,(struct sockaddr *) &cliaddr, &len);
      buffer[n]='\0';
      struct val_req_players v;
      if(!(strcmp("match",buffer)))
      {
        sendto(sockfd,(const char *)"All servers busy!!",strlen("All servers busy!!"),MSG_CONFIRM,(const struct *)&cliaddr,len);
      }
      memset(&cliaddr, 0, sizeof(cliaddr));
      memset(&cliaddr2, 0, sizeof(cliaddr2));
    }
  }
}
