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
	char *newfifo1 = "/tmp/newfifo1";
	mkfifo(newfifo1,0666);
	char buffer[100];
	int fd = open(newfifo1,O_WRONLY);
	while(1)
	{
		printf("inside P2");
		cout<<"Enter the string to enter\n";
		cin>>buffer;
		cout<<"FD =" <<fd<<endl;
		write(fd,buffer,strlen(buffer));
		printf("Buffer  = %s",buffer);
		sleep(5);
	}
}
