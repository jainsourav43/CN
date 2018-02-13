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
using namespace std;
int main()
{
	char * newfifo = "/tmp/newfifo";
	mkfifo(newfifo,0666);
	int fd = open("input.txt",O_RDONLY);


	
	while(1)
	{
	//	printf("inside P1");
		char buffer[10];
		read(fd,buffer,5);
		int fd1=  open(newfifo,O_WRONLY|O_NONBLOCK);
		cout<<"FD1"<<fd1<<endl;
		write(fd1,buffer,strlen(buffer));
		cout<<"Fd"<<endl;
		close(fd1);
		//cout<<buffer<<endl;
		sleep(1);
	}
	
}

