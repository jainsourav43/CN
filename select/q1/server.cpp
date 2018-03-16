#include <bits/stdc++.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string>
using namespace std;

void itoa(char* str,int n)
{
	int i = 0;
	while(n)
	{
		int rem = n%10;
		str[i] = (rem+'0');
		n /= 10;
		i++;
	}	
	str[i] = '\0';
	reverse(str,str+i);
}

void self(char* buf)
{
	int n1 = 0;
	int n2 = 0;
	int i=2;
	for(;buf[i]!=' ';i++)
	{
		n1 *= 10;
		n1 += buf[i]-'0';
	}
	i++;
	for(;buf[i]!='*';i++)
	{
		n2 *= 10;
		n2 += buf[i]-'0';
	}
	switch(buf[0])
	{
		case 'A':
		{
			cout<<n1<<"+"<<n2<<"="<<n1+n2<<endl;
			break;
		}
		case 'S':
		{
			cout<<n1<<"-"<<n2<<"="<<n1-n2<<endl;
			break;
		}
		case 'D':
		{
			cout<<n1<<"/"<<n2<<"="<<n1/n2<<endl;
			break;
		}
		case 'M':
		{
			cout<<n1<<"*"<<n2<<"="<<n1*n2<<endl;
			break;
		}
	}
}

void proc(char* buf)
{
	int n1 = 0;
	int n2 = 0;
	int i=2;
	for(;buf[i]!=' ';i++)
	{
		n1 *= 10;
		n1 += buf[i]-'0';
	}
	i++;
	for(;buf[i]!='*';i++)
	{
		n2 *= 10;
		n2 += buf[i]-'0';
	}
	char num1[10],num2[10];
	itoa(num1,n1);
	itoa(num2,n2);
	switch(buf[0])
	{
		case 'A':
		{
			int c = fork();
			if(c==0)
			{
				char* args[] = {(char*)"./adder",num1,num2};
				execvp(args[0],args);
			}
			else
				return;
			break;
		}
		case 'S':
		{
			int c = fork();
			if(c==0)
			{
				char* args[] = {(char*)"./subtracter",num1,num2};
				execvp(args[0],args);
			}
			else
				return;
			break;
		}
		case 'D':
		{
			int c = fork();
			if(c==0)
			{
				char* args[] = {(char*)"./divider",num1,num2};
				execvp(args[0],args);
			}
			else
				return;
			break;
		}
		case 'M':
		{
			int c = fork();
			if(c==0)
			{
				char* args[] = {(char*)"./multiplier",num1,num2};
				execvp(args[0],args);
			}
			else
				return;
			break;
		}
	}
	
	exit(0);
}

int main()
{
	char* t1 = (char*)"/tmp/t1";
	char* t2 = (char*)"/tmp/t2";
	mkfifo(t1,0666);
	mkfifo(t2,0666);
	int fd[2];
	fd[0] = open(t1,O_RDONLY);
	fd[1] = open(t2,O_RDONLY);
	fd_set selfd;
	while(true)
	{
		FD_ZERO(&selfd);
		FD_SET(fd[0],&selfd);
		FD_SET(fd[1],&selfd);
		int ret = select(10,&selfd,NULL,NULL,NULL);
		if(ret > 0)
		{
			if(FD_ISSET(fd[0],&selfd))
			{
				char buf[100];
				bzero(buf,100);
				read(fd[0],buf,100);
				self(buf);
			}
			if(FD_ISSET(fd[1],&selfd))
			{
				char buf[100];
				bzero(buf,100);
				read(fd[1],buf,100);
				proc(buf);
			}
		}
	}
	
	close(fd[0]);
	close(fd[1]);
	
	return 0;
}
