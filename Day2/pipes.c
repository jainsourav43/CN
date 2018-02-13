#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<string.h>
#include <iostream>
using namespace std;
void parentfun(int wfd)
{
	char s[50];
	
		printf("Enter the String \n");
		scanf("%s",s);
		write(wfd,s,strlen(s));
	
}
void childfun(int rfd)
{
	char s[50];
	

		read(rfd,s,strlen(s));
		printf(" String is %s  \n",s);
		
		
	
}
int main(int argc, char const *argv[])
{
	int c;
	int pfd[2];
	pipe(pfd);
	c=fork();
	int x=0;
	if(c>0)
	{
		
		close(pfd[0]);
		parentfun(pfd[1]);
		x=1;
	}
	else
	{
		while(x==0);
		close(pfd[1]);
		childfun(pfd[0]);
	}
	return 0;
}