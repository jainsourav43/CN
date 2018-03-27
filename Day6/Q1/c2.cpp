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
int main()
{
	int tos;
	char *fifo1="/tmp/fifo1";
	char *fifo2 = "/tmp/fifo2";
	char *fifo3 = "/tmp/fifo3";
	int fd1 = open(fifo1,O_WRONLY);
	int fd2 = open(fifo2,O_WRONLY);

	while(1)
	{
		cout<<"Enter the Type of Service you want\n";
		cin>>tos;
		if(tos==1)
		{
			cout<<"fd1,fd2,fd3 = "<<fd1<<"  "<<fd2<<"  "<<endl;
			char buf1[2];
			cout<<"Enter the Service \n";
			cin>>buf1;
			write(fd1,buf1,1);
		}
		else if(tos=2)
		{
			int fd3  = open(fifo3,O_RDONLY);
			cout<<"fd1,fd2,fd3 = "<<fd1<<"  "<<fd2<<"  "<<fd3<<endl;
			cout<<"Enter the Service \n";
			char buf1[2];
			cin>>buf1;
			write(fd2,buf1,1);
			char buf[100];
			read(fd3,buf,10);

		}
	}
}
