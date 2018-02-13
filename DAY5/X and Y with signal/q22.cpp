#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;
#define SHMSZ     27
int shmid1,shmid2;
key_t keyX,keyY;
char *shmX,*shmY, *s;
void itoa(int a,char* buffer,int base)
{
	int i=0;
	char temp[100];
	while(a!=0)
	{

		temp[i++]= (a%10) + '0';
		a=a/10;
	}
	temp[i] = '\0';
	int   j=0;
	for(i=strlen(temp)-1;i>=0;i--)
	{
		buffer[j++]= temp[i];

	}
	buffer[j]= '\0';
}
void runnerread2(int sig)
{
	int x  = atoi(shmX);
	x=x+1;
	char buffer[100];
	itoa(x,buffer,10);
	sprintf(shmY,buffer);
	cout<<"Y= "<<shmY<<endl;
	//cout<<"Pod = "<<getpid()<<endl;
	sleep(1);
	kill(getppid(),SIGUSR1);



}
int main(int argc, char  *argv[])
{
    keyX = 5678;
    keyY = 5679;
    shmid1 = shmget(keyX, SHMSZ, IPC_CREAT | 0666);
    shmid2 = shmget(keyY, SHMSZ, IPC_CREAT | 0666);
    shmX = (char*)shmat(shmid1, NULL, 0);
	shmY = (char*)shmat(shmid2, NULL, 0);
	signal(SIGUSR2,runnerread2);
	kill(getppid(),SIGUSR1);
	while(1);

	return 0;
}