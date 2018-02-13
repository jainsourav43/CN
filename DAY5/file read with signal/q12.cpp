#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;
int c=0;
void runnerread2(int sig)
{
	int i=0;
	char buffer[1000],line[1000];
	int fd;
	dup2(0,fd);
	if(read(fd,buffer,1)==0)
	{
		return;
	}
	while(*buffer!='\n' )
	{
		line[i++] = *buffer;
		if(read(fd,buffer,1)==0)return;
		
	}
	dup2(fd,0);
	cout<<sig<<"Child "<<line<<endl;
	cout<<kill(getppid(),SIGUSR1);
	cout.flush();


}
int main(int argc, char *argv[])
{
	
	int fd = open("f1.txt",O_RDONLY);
	signal(SIGUSR2,runnerread2);
	kill(getppid(),SIGUSR1);
	while(1);
	return 0;
}
