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
	while(1)
	{
	//	printf("inside P3");
		kill(getppid(),SIGUSR1);
	//	sleep(1);
	}
}
