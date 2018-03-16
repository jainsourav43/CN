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
#include <pthread.h>
using namespace std;

int nt1,nt2;

void* adder(void* args)
{
	cout<<"ADDER: "<<nt1<<"+"<<nt2<<"="<<nt1+nt2<<endl;
}

void* subtracter(void* args)
{
	cout<<"SUBTRACTER: "<<nt1<<"-"<<nt2<<"="<<nt1-nt2<<endl;
}

void* multiplier(void* args)
{
	cout<<"MULTIPLIER: "<<nt1<<"*"<<nt2<<"="<<nt1*nt2<<endl;
}

void* divider(void* args)
{
	cout<<"DIVIDER: "<<nt1<<"/"<<nt2<<"="<<nt1/nt2<<endl;
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

void thrd(char* buf)
{
	nt1 = 0;
	nt2 = 0;
	int i=2;
	for(;buf[i]!=' ';i++)
	{
		nt1 *= 10;
		nt1 += buf[i]-'0';
	}
	i++;
	for(;buf[i]!='*';i++)
	{
		nt2 *= 10;
		nt2 += buf[i]-'0';
	}
	
	switch(buf[0])
	{
		case 'A':
		{
			pthread_t pt;
			pthread_create(&pt,NULL,adder,NULL);	
			break;
		}
		case 'S':
		{
			pthread_t pt;
			pthread_create(&pt,NULL,subtracter,NULL);	
			break;
		}
		case 'M':
		{
			pthread_t pt;
			pthread_create(&pt,NULL,multiplier,NULL);	
			break;
		}
		case 'D':
		{
			pthread_t pt;
			pthread_create(&pt,NULL,divider,NULL);
			break;
		}
	}
	
}

int main()
{
	char* t1 = (char*)"/tmp/tt1";
	char* t2 = (char*)"/tmp/tt2";
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
				thrd(buf);
			}
		}
	}
	
	close(fd[0]);
	close(fd[1]);
	
	return 0;
}
