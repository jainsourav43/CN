#include<string>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<semaphore.h>
#include<cstdlib>
#include<bits/stdc++.h>

#include<unistd.h>
#include<poll.h>
#include<pthread.h>
using namespace std;
int main(int argc, char const *argv[])
{
	cout<<"Inside FirstP3\n";
	string pipeNames[]={"./pipe1","./pipe2","./pipe3"};
	struct pollfd pfd[3];
	mkfifo(pipeNames[2].c_str(),0666);
	int fd = open(pipeNames[2].c_str(),O_WRONLY); 
	cout<<"InsideP3\n";
	while(1)
	{
		cout<<"Back in P3\n";
		write(fd,"HelloP3",7);
		sleep(3);
	}
	return 0;
}
