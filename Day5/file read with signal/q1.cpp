#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;
int c=0;
void runnerread1(int sig)
{
	int i=0;
	char buffer[20],line[1000];
	int fd;
	bzero(buffer,20);
	dup2(0,fd);
	if(read(fd,buffer,1)==0)
	{
		return;
	}
			cout<<"sSTrlen= "<<strlen(buffer)<<endl;

	while(*buffer!='\n' )
	{
		line[i++] = *buffer;
		if(read(fd,buffer,1)==0)return;
		cout<<"strlen= "<<strlen(buffer)<<endl;
	}
	dup2(fd,0);
	cout<<sig<<"Parent "<<line<<endl;
	cout<<kill(c,SIGUSR2);
	cout.flush();


}


int main(int argc, char *argv[])
{
	
	int fd = open("f1.txt",O_RDONLY);
	dup2(fd,0);
	signal(SIGUSR1,runnerread1);
	c=fork();
	if(c>0)
	{
		wait(NULL);
	}
	else
	{
	
		execvp("./q12",NULL);
	}
	return 0;
}
