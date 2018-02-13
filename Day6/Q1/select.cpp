#include<bits/stdc++.h>
#include<stdio.h>
#include<cstring>
#include<string>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<semaphore.h>
#include<cstdlib>
#include<unistd.h>
#include<poll.h>
#include<pthread.h>
#include<sys/select.h>
using namespace std;
void s1()
{
cout<<"Giving Service S1"<<endl;
}
void s2()
{
cout<<"Giving Service S2"<<endl;
}
void s3()
{
cout<<"Giving Service S3"<<endl;
}
int main()
{
	int t;
	char *fifo1 = "/tmp/fifo1";
	char *fifo2 = "/tmp/fifo2";
	mkfifo(fifo1,0666);
	mkfifo(fifo2,0666);
	cout<<"I am Sourav\n";
	
	int fd1 = open(fifo1,O_RDONLY);
	cout<<"Hello\n";
	int fd2 = open(fifo2,O_RDONLY);
	cout<<"Baby\n";
	fd_set rfds[2];
	struct timeval tv ;
	tv.tv_sec=2;
	tv.tv_usec=10;
	
	//FD_ZERO(&rfds[0]);
	//FD_ZERO(&rfds[1]);
	FD_SET(fd1,&rfds[0]);
	FD_SET(fd2,&rfds[1]);
	while(1)
	{
		cout<<"inside While\n";
		int retval = select(20,rfds,NULL,NULL,&tv);
		if(retval==0)
		{
			cout<<"Timeout\n";
			
		}
		if(FD_ISSET(fd1,&rfds[0]))
		{
			
		    cout<<"Inside1"<<endl;
			char buf[2];
			read(fd1,buf,1);
			buf[1]='\0';
			cout<<"Buf = "<<buf<<endl;
			int ser=atoi(buf);
			if(ser==1)
			{
				s1();
			}
			else if(ser==2)
			{
				s2();
			}
			else if(ser==3)
			{
				s3();
			}
			else
			{
				cout<<"Invalid Service\n";
			}
		}
		if(FD_ISSET(fd1,&rfds[1]))
		{
		    cout<<"Inside2"<<endl;
		    
			execvp("./s4",NULL);
			cout<<"after "<<endl;
			
			
		}
	}
	
	
	
	
	
	
	
	
	
}
