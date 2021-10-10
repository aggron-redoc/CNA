#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define INT_MAX 2147483647

void XOtable(char *x[])
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
 close(sockfd);
 struct sockaddr_in servaddr2;
 if ( (sockfd2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
  }
 memset(&servaddr2, 0, sizeof(servaddr2));
 servaddr2.sin_family = AF_INET;
 servaddr2.sin_port = htons(porter);
 servaddr2.sin_addr.s_addr = inet_addr(argv[1]);
 int result=1,i=0,m,u;
 char *x[]={"___", "___", "___"};
 XOtable(x);
 printf("hi\n");
 printf("%d %d",flag,i);
 while(result==1)
 {
   if((i)%2==0 && flag==1)
   {
     printf("Why??");
     char bufferr[1024];
     printf("Why??");
     recvfrom(sockfd2,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr2, &len);
     printf("Waiting\n");
     printf("%s",buffer);
     scanf("%d %d",&m,&u);
     x[m][u]='X';
     XOtable(x);
     char coos[20];
     sprintf(coos,"%d %d",m,u);
     sendto(sockfd2,coos,sizeof(coos),0,(struct sockaddr *) &servaddr2, len);
   }
   else if((i)%2==0 && flag==0)
   {
     printf("Why??");
     char buffer[1024];
     recvfrom(sockfd2,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr2, &len);
     sscanf(buffer,"%d %d",&m,&u);
     x[m][u]='X';
     printf("Opponent's move: \n");
     XOtable(x);
   }
   else if((i)%2==1 && flag==1)
   {
     printf("Why??");
     char buffer[1024];
     recvfrom(sockfd2,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr2, &len);
     sscanf(buffer,"%d %d",&m,&u);
     x[m][u]='O';
     printf("Opponent's move: \n");
     XOtable(x);
   }
   else if((i)%2==1 && flag==0)
   {
     printf("Why??");
     char buffer[1024];
     recvfrom(sockfd2,buffer,sizeof(buffer),0,(struct sockaddr *) &servaddr2, &len);
     printf("%s",buffer);
     scanf("%d %d",&m,&u);
     x[m][u]='O';
     XOtable(x);
     char coos[20];
     sprintf(coos,"%d %d",m,u);
     sendto(sockfd2,coos,sizeof(coos),0,(struct sockaddr *) &servaddr2, len);
   }
   char sresult[10];
   recvfrom(sockfd2,sresult,sizeof(sresult),0,(struct sockaddr *)&servaddr2,&len);
   sscanf(sresult,"%d",&result);
   i++;
 }
 printf("bye");
 recvfrom(sockfd2,buffer,sizeof(buffer),0,(struct sockaddr *)&servaddr2,&len);
 printf("%s\n",buffer);
 close(sockfd2);
}
