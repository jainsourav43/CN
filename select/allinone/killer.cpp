#include<bits/stdc++.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/wait.h>

using namespace std;

int pid;

void killit(int sig)
{
	kill(pid,SIGUSR1);
	cout<<"kill"<<endl;
	signal(SIGINT,killit);
}

int main()
{
	signal(SIGINT,killit);
	int sid = shmget(5678,10,IPC_CREAT | 0666);
	char* shm = (char*)shmat(sid,NULL,0);
	char* stars = (char*)"****";
	sprintf(shm,stars,strlen(stars));
	while(shm[0]=='*');
	sleep(1);
	pid = atoi(shm);
	while(true);
}
