#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<string.h>
#include <iostream>
#include<sys/wait.h>
using namespace std;
void parentfun(int wfd)
{
	
		char s[50];
		printf("Enter the String \n");
		cin>>s;
		cout<<"S is "<<s<<endl;
		write(wfd,s,strlen(s));
	
}
void childfun(int rfd)
{
		char *s;
		read(rfd,s,50);
		cout<<"s = "<<s<<endl;
}
int main(int argc, char const *argv[])
{
	int c;
	int pfd[2];
	pipe(pfd);
	c=fork();
	if(c>0)
	{
		
		close(pfd[0]);
		parentfun(pfd[1]);
	}
	else
	{
		wait(NULL);
		close(pfd[1]);
		childfun(pfd[0]);
	}
	return 0;
}