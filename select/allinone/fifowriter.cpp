#include <bits/stdc++.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
int main()
{
	char* newfifo = (char*)"/tmp/aiofifo";
	cout<<"hh"<<endl;
	mkfifo(newfifo,0666);
	int ffd = open(newfifo,O_WRONLY);
	char* msg = (char*)"Writer's message in FIFO\n";
	cout<<"here"<<endl;
	while(true)
	{
		write(ffd,msg,strlen(msg));
		sleep(4);
	}
	
	return 0;
}
