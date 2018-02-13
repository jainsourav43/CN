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
	char fifo3 = "/tmp/fifo3";
	mkfifo(fifo3,0666);
	while(1)
	{
		char buf[200];
		cout<<"Enter the Data to write\n";
		cin>>buf;
		int fd= open(fifo3,O_WRONLY);
		write(fd,buf,strlen(buf);
		close(fd);
	}
}
