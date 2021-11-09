#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
  FILE *p=fopen("sumatext.txt","a");
  fprintf(p,"good to see you!\n");
  fclose(p);
}
