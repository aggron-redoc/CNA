#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<pthread.h>

struct val_req_players
{
  struct sockaddr_in players[2];
  int porter;
};

int av_ports[3][3]={{8090,1},{8092,1}};
int flag=0;
int n;
socklen_t len;
char buffer[1024];
int refree(char *ttt[])
{
  //1 for game in line
  //0 for draw
  //2 for player1 win
  //3 for player2 win
  int count=0;
  for(int i=0;i<3;i++)
    for(int j=0;j<sizeof(ttt[i]);j++)
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
  struct val_req_players *arg2 = (struct val_req_players *)arg;
  char *x[]={"___", "___", "___"};
  int gamesock;
  struct sockaddr_in servaddr;
  if ( (gamesock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
  {
       perror("socket creation failed");
       exit(EXIT_FAILURE);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  struct sockaddr_in player1=arg2->players[0];
  struct sockaddr_in player2=arg2->players[1];
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  int porter=arg2->porter;
  servaddr.sin_port = htons(porter);
  if ( bind(gamesock, ( struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  int i=0;int m,u;int result=1;
  while((result=refree(x))==1)
  {
    char bufferr[50];
    if((i++)%2==0)
    {
      n=recvfrom(gamesock,buffer,sizeof(buffer),0,(struct sockaddr *) &player1, &len);
      buffer[n]='\0';
      char bufferrr[100];
      sscanf(bufferrr,"%d %d",&m,&u);
      x[m][u]='X';
      sendto(gamesock,bufferrr,sizeof(buffer),0,( struct sockaddr *) &player2,len);
    }
    else
    {
      n=recvfrom(gamesock,buffer,sizeof(buffer),0,(struct sockaddr *) &player2, &len);
      buffer[n]='\0';
      char bufferrr[100];
      sscanf(bufferrr,"%d %d",&m,&u);
      x[m][u]='O';
      sendto(gamesock,bufferrr,sizeof(buffer),0,( struct sockaddr *) &player1,len);
    }
    sprintf(bufferr,"%d",result);
    sendto(gamesock,bufferr,sizeof(bufferr),0,( struct sockaddr *) &player1,len);
    sendto(gamesock,bufferr,sizeof(bufferr),0,( struct sockaddr *) &player2,len);
  }
  if(result==2)
  {
    sendto(gamesock,"X won",sizeof("X won"),0,( struct sockaddr *) &player1,len);
    sendto(gamesock,"X won",sizeof("X won"),0,( struct sockaddr *) &player2,len);
  }
  else if(result==3)
  {
    sendto(gamesock,"O won",sizeof("X won"),0,( struct sockaddr *) &player1,len);
    sendto(gamesock,"O won",sizeof("X won"),0,( struct sockaddr *) &player2,len);
  }
  else if(result==0)
  {
    sendto(gamesock,"Draw",sizeof("X won"),0,( struct sockaddr *) &player1,len);
    sendto(gamesock,"Draw",sizeof("X won"),0,( struct sockaddr *) &player2,len);
  }
  if(av_ports[0][0]==porter)
    av_ports[0][1]=1;
  else if(av_ports[1][0]==porter)
    av_ports[1][1]=1;
  close(gamesock);
  flag=0;
  pthread_exit(NULL);
  return NULL;
}

void main()
{
  int sockfd;
  //tic tac toe
  struct sockaddr_in servaddr, cliaddr,cliaddr2;
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
  {
       perror("socket creation failed");
       exit(EXIT_FAILURE);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));
  memset(&cliaddr2, 0, sizeof(cliaddr2));
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(5000);
  if ( bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  while(1)
  {
    if(flag==0)
    {
      int len,n;
      n=recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &cliaddr, &len);
      printf("%s\n",inet_ntoa(cliaddr.sin_addr));
      if(!(strcmp("match",buffer)))
      {
        printf("hi\n");
        for(int i=0;i<2;i++)
          if(av_ports[i][1]==1)
          {
            flag=1;av_ports[i][1]=2;
            i+=2;
          }
      }
      if(flag==1)
      {
        int sflag=sendto(sockfd,"Waiting for your opponent to join",sizeof("Waiting for your opponent to join"),0,(struct sockaddr *)&cliaddr,len);
        printf("sent%d\n",sflag);
      }
    }
    else if(flag==1)
    {
      socklen_t len2;
      memset(&cliaddr2,0,sizeof(cliaddr2));
      n=recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*) &cliaddr2, &len2);
      printf("Client2: %s\n",inet_ntoa(cliaddr2.sin_addr));
      struct val_req_players v;
      if(!(strcmp("match",buffer)))
      {
        printf("Hello\n");
        for(int i=0;i<2;i++)
          if(av_ports[i][1]==2)
          {
            flag=0;av_ports[i][1]=0;
            i+=2;
            v.porter=av_ports[i][0];
          }
        char port_string[50];sprintf(port_string,"%d",v.porter);
        sendto(sockfd,"You are O!",sizeof("You are O!"),0,(struct sockaddr *)&cliaddr2,len2);
        sendto(sockfd,"Opponent found! Initiating the match!",sizeof("Opponent found! Initiating the match!"),0,(struct sockaddr *)&cliaddr,len);
        sendto(sockfd,"Opponent found! Initiating the match!",sizeof("Opponent found! Initiating the match!"),0,( struct sockaddr *)&cliaddr2,len2);
        sendto(sockfd,port_string,sizeof(port_string),0,( struct sockaddr *)&cliaddr,len);
        sendto(sockfd,port_string,sizeof(port_string),0,( struct sockaddr *)&cliaddr2,len2);
        v.players[0]=cliaddr;
        v.players[1]=cliaddr2;
        pthread_t x;
        if(av_ports[0][1]==0 && av_ports[1][1]==0)flag=-1;
        pthread_create(&x, NULL, &tictactoe, &v);
      }
      memset(&cliaddr, 0, sizeof(cliaddr));
      memset(&cliaddr2, 0, sizeof(cliaddr2));
    }
    else
    {
      n=recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &cliaddr, &len);
      buffer[n]='\0';
      struct val_req_players v;
      if(!(strcmp("match",buffer)))
      {
        sendto(sockfd,"All servers busy!!",sizeof("All servers busy!!"),0,( struct sockaddr *)&cliaddr,len);
      }
      memset(&cliaddr, 0, sizeof(cliaddr));
      memset(&cliaddr2, 0, sizeof(cliaddr2));
    }
  }
}
