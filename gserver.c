#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<pthread.h>
#define AV_PORTS 3
#define INT_MAX 2147483647

struct val_req_players
{
  struct sockaddr_in players[2];
  int porter;
};


//this function checks and tells whether game has ended or not
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
//this is the thread where the game is run!!
int av_ports[][2]={{8090,1},{8091,1},{8092,1},{8093,1}};
void* tictactoe(void *arg)
{
  struct val_req_players *arg2 = (struct val_req_players *)arg;
  printf("Inside thread\n");
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
  free(arg2);
  servaddr.sin_port = htons(porter);
  printf("Port for this thread: %d\n",porter);
  if ( bind(gamesock, ( struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  int i=-1;int m,u;int result=1;
  socklen_t len=sizeof(player1),len2=sizeof(player2);
  while((result=refree(x))==1)
  {
    if((++i)%2==0)//x
    {
      printf("Turn of: %s %d\n",inet_ntoa(player1.sin_addr),ntohs(player1.sin_port));
      int n;
      sendto(gamesock,"Coordinates for the next 'X': ",0,sizeof("Coordinates for the next 'X': "),(struct sockaddr *) &player1,len);
      printf("SENT!!\n");
      char bufferr[15];
      recvfrom(gamesock,bufferr,sizeof(bufferr),0,(struct sockaddr *)&player1, &len);
      sscanf(bufferr,"%d %d",&m,&u);
      x[m][u]='X';
      sendto(gamesock,bufferr,sizeof(bufferr),0,(struct sockaddr *)&player2,sizeof(len2));
    }
    else
    {
      printf("Turn of: %s %d\n",inet_ntoa(player2.sin_addr),ntohs(player2.sin_port));
      int n;
      sendto(gamesock,"Coordinates for the next 'O': ",sizeof("Coordinates for the next 'O': "),0,(struct sockaddr *)&player2,len2);
      char bufferr[15];
      recvfrom(gamesock,bufferr,sizeof(bufferr),0,(struct sockaddr *)&player2, &len2);
      sscanf(bufferr,"%d %d",&m,&u);
      x[m][u]='O';
      sendto(gamesock,bufferr,sizeof(bufferr),0,(struct sockaddr *)&player1,len);
    }
    char sresult[10];
    sprintf(sresult,"%d",result);
    sendto(gamesock,sresult,sizeof(sresult),0,(struct sockaddr *)&player2,sizeof(player2));
    sendto(gamesock,sresult,sizeof(sresult),0,(struct sockaddr *)&player1,sizeof(player1));
  }
  if(result==2)
  {
    sendto(gamesock,"X won",sizeof("X won"),0,( struct sockaddr *) &player1,sizeof(player1));
    sendto(gamesock,"X won",sizeof("X won"),0,( struct sockaddr *) &player2,sizeof(player2));
  }
  else if(result==3)
  {
    sendto(gamesock,"O won",sizeof("X won"),0,( struct sockaddr *) &player1,sizeof(player1));
    sendto(gamesock,"O won",sizeof("X won"),0,( struct sockaddr *) &player2,sizeof(player2));
  }
  else if(result==0)
  {
    sendto(gamesock,"Draw",sizeof("X won"),0,( struct sockaddr *) &player1,sizeof(player1));
    sendto(gamesock,"Draw",sizeof("X won"),0,( struct sockaddr *) &player2,sizeof(player2));
  }
  for(int i=0;i<AV_PORTS;i++)
    if(av_ports[i][0]==porter)
    {
      av_ports[i][1]=1;
      i=INT_MAX;
    }
  pthread_detach(pthread_self());
  pthread_exit(NULL);
  return NULL;
}
void main()
{
  struct sockaddr_in waiting_player;
  socklen_t waiting_player_len;
  int sockfd;
  struct sockaddr_in servaddr,cliaddr;
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
  {
       perror("socket creation failed");
       exit(EXIT_FAILURE);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(5000);
  if ( bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  pthread_t match;
  while(1)
  {
    socklen_t len;int n;
    char buffer[1024];
    n=recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &cliaddr, &len);
    buffer[n]='\0';
    printf("Received request from: %s %d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
    int i;
    for(i=0;i<AV_PORTS;i++)
    {
      if(av_ports[i][1]==2)
      {
        av_ports[i][1]=0;
        struct val_req_players *for_thread=malloc(sizeof(struct val_req_players));
        for_thread->players[0]=waiting_player;
        for_thread->players[1]=cliaddr;
        for_thread->porter=av_ports[i][0];
        i=INT_MAX-1;
        //printf("%d",i);
        sendto(sockfd,"pair_ready",sizeof("pair_ready"),0,(struct sockaddr *)&cliaddr,len);
        char portnumber[20];
        sprintf(portnumber,"%d",for_thread->porter);
        sendto(sockfd,portnumber,sizeof(portnumber),0,(struct sockaddr *) &waiting_player,waiting_player_len);
        sendto(sockfd,portnumber,sizeof(portnumber),0,(struct sockaddr *) &cliaddr,len);
        printf("Designated Ports sent to both parties\n");
        memset(&waiting_player,0,sizeof(waiting_player));
        int waiting_player_len=-1;
        pthread_create(&match, NULL, &tictactoe,for_thread);
      }
    }
    if(i!=INT_MAX)
      for (i = 0; i < AV_PORTS; i++)
      {
        if(av_ports[i][1]==1)
        {
          av_ports[i][1]=2;
          waiting_player=cliaddr;
          waiting_player_len=len;
          i=INT_MAX-2;
          sendto(sockfd,"waiting",sizeof("waiting"),0,(struct sockaddr *)&cliaddr,len);
          printf("One Party waiting!!\n");
        }
      }
    if(i!=INT_MAX-1 && i!=INT_MAX)
    {
      sendto(sockfd,"Server Busy!!",sizeof("Server Busy!!"),0,(struct sockaddr *)&cliaddr,len);
    }
  }
}
