#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define INT_MAX 2147483647

void XOtable(char x[][4])
{
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
     printf("%c ",x[i][j]);
   printf("\n");
  }
}

void main(int argc, char **argv)
{
  int sockfd,sockfd2;int flag=0;
  char buffer[1024];
  struct sockaddr_in servaddr;
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
       perror("socket creation failed");
       exit(EXIT_FAILURE);
   }
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 int porter;
 sscanf(argv[2],"%d",&porter);
 servaddr.sin_port = htons(porter);
 servaddr.sin_addr.s_addr = inet_addr(argv[1]);
 int n;
 socklen_t len=sizeof(servaddr);
 //request to the server
 sendto(sockfd,"X",sizeof("X"),0,(struct sockaddr *) &servaddr,len);
 recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr,&len);
 if(!strcmp(buffer,"waiting"))
 {
   printf("Please wait for an opponent!!\nYou are X\n");flag=1;
 }
 else if(!strcmp(buffer,"pair_ready"))
 {
   printf("You are O\n");flag=0;
 }
 else
 {
   printf("%s\nPlease Try again after sometime!!",buffer);
   return;
 }

 recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr,&len);
 sscanf(buffer,"%d",&porter);
 printf("Port alloted: %d\n",porter);
 servaddr.sin_port=htons(porter);
 int result=1,i=0,m=0,u=0;
 char x[][4]={"___", "___", "___"};
 XOtable(x);
 while(result==1)
 {
   if(i%2==0 && flag==1)
   {
     printf("Turn of: %s %d\n",inet_ntoa(servaddr.sin_addr),ntohs(servaddr.sin_port));
     recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr, &len);
     //printf("Waiting\n");
     int valid_move=0;
     while(!valid_move)
     {
      printf("%s",buffer);
      scanf("%d %d",&m,&u);
      if(m>=0 && m<=2 && u>=0 && u<=2 && x[m][u]=='_')valid_move=1;
      else printf("Invalid Move.Please Try again!!\n");
     }
     x[m][u]='X';
     printf("\n");
     XOtable(x);
     char coos[20];
     sprintf(coos,"%d %d",m,u);
     sendto(sockfd,coos,sizeof(coos),0,(struct sockaddr *) &servaddr, len);
   }
   else if(i%2==0 && flag==0)
   {
     recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr, &len);
     sscanf(buffer,"%d %d",&m,&u);
     x[m][u]='X';
     printf("Opponent's move: \n");
     XOtable(x);
   }
   else if(i%2==1 && flag==1)
   {
     recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr, &len);
     sscanf(buffer,"%d %d",&m,&u);
     x[m][u]='O';
     printf("Opponent's move: \n");
     XOtable(x);
   }
   else if(i%2==1 && flag==0)
   {
     recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr, &len);
     printf("%s",buffer);
     int valid_move=0;
     while(!valid_move)
     {
      printf("%s",buffer);
      scanf("%d %d",&m,&u);
      if(m>=0 && m<=2 && u>=0 && u<=2 && x[m][u]=='_')valid_move=1;
      else printf("Invalid Move.Please Try again!!\n");
     }
     printf("\n");
     x[m][u]='O';
     XOtable(x);
     char coos[20];
     sprintf(coos,"%d %d",m,u);
     sendto(sockfd,coos,sizeof(coos),0,(struct sockaddr *) &servaddr, len);
   }
   char sresult[10];
   recvfrom(sockfd,sresult,sizeof(sresult),0,(struct sockaddr *)&servaddr,&len);
   sscanf(sresult,"%d",&result);
   i++;
 }
 //printf("bye");
 recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&servaddr,&len);
 printf("%s\n",buffer);
 close(sockfd);
}
