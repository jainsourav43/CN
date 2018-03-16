#include<string>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<semaphore.h>
#include<cstdlib>
#include<unistd.h>
#include<poll.h>
#include<pthread.h>
#include<bits/stdc++.h>

using namespace std;
int main(int argc, char const *argv[])
{
		cout<<"Inside FirstP1\n";
	string pipeNames[]={"./pipe1","./pipe2","./pipe3"};
	mkfifo(pipeNames[0].c_str(),0666);
	int fd = open(pipeNames[0].c_str(),O_WRONLY); 
	cout<<"InsideP1\n";
	while(1)
	{
		cout<<"Back in P1\n";
		write(fd,"HelloP1",7);
		sleep(3);
	}
	return 0;
}
