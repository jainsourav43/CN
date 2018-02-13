#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
  int c=0;
  printf("This is parent program\n");
  c=fork();

  if(c>0)
  {
    printf("This is program P1\n");    wait();
  }
  else
  {
    int d=fork();
    if(d>0)
    {
      printf("This is program P2\n"); wait();
    }
    else
    {
      printf("This is program P3\n");
      execlp("/home/shoumorup/Computer Networks/P4.c","P4.c",NULL);
    }
  }
 return 0;
}  
