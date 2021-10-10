#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


char *SERVER_ADDR="127.0.0.1";


void main()
{
  int sockfd;int flag=0;
  char buffer[1024];
  struct sockaddr_in servaddr;
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
       perror("socket creation failed");
       exit(EXIT_FAILURE);
   }
 memset(&servaddr, 0, sizeof(servaddr));
 int porter=5000;
 servaddr.sin_family = AF_INET;
 servaddr.sin_port = htons(porter);
 servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
 int n;
 socklen_t len;
 char temp[]="match";
 sendto(sockfd, temp, sizeof(temp),0, ( struct sockaddr *) &servaddr,sizeof(servaddr));
 printf("%d\n",n);
 n = recvfrom(sockfd, buffer, sizeof(buffer),0, (struct sockaddr *) &servaddr,&len);
 buffer[n]='\0';
 printf("%d\n",n);
 printf("%s\n",buffer);
 if((strcmp(buffer,"You are O!")) && strcmp(buffer,"All servers busy!!"))
 {
   printf("You are X\n");
   flag=1;sleep(30);
 }
 n = recvfrom(sockfd, buffer, sizeof(buffer),0, (struct sockaddr *) &servaddr,&len);
 buffer[n]='\0';
 printf("%s\n",buffer);
 n = recvfrom(sockfd, buffer, sizeof(buffer),0, (struct sockaddr *) &servaddr,&len);
 buffer[n]='\0';
 sscanf(buffer,"%d",&porter);
 printf("Port alloted in server: %d\n",porter);
 servaddr.sin_port = htons(porter);
 int result=1;
 char typer=(flag==1)?('X'):('O');
 char *x[]={"___", "___", "___"};
 while(result==1)
 {
   int m,u;
   printf("Give co-ordinates for %c: ",typer);
   scanf("%d %d",&m,&u);
   x[m][u]='X';
   char bufferr[50];sprintf(bufferr,"%d %d",m,n);
   sendto(sockfd,bufferr, sizeof(bufferr),0,( struct sockaddr *) &servaddr,sizeof(servaddr));
   for(int i=0;i<3;i++)
   {
     for(int j=0;j<3;j++)
      printf("%c ",x[i][j]);
    printf("\n");
   }
   n = recvfrom(sockfd, buffer, sizeof(buffer),0, (struct sockaddr *) &servaddr,&len);
   buffer[n]='\0';
   sscanf(buffer,"%d %d",&m,&u);
   x[m][u]='O';
   for(int i=0;i<3;i++)
   {
     for(int j=0;j<3;j++)
      printf("%c ",x[i][j]);
    printf("\n");
   }
   n = recvfrom(sockfd,buffer, sizeof(buffer),0, (struct sockaddr *) &servaddr,&len);
   buffer[n]='\0';
   sscanf(buffer,"%d",&result);
 }
 n = recvfrom(sockfd, buffer, sizeof(buffer),0, (struct sockaddr *) &servaddr,&len);
 buffer[n]='\0';
 printf("%s\nGame Over!!",buffer);
 close(sockfd);
}
