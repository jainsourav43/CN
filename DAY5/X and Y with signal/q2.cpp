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
int c;
int shmid1,shmid2;
key_t keyX,keyY;
char *shmX, s[100],*shmY; 
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
void runnerread1(int sig)
{
//	cout<<"Before y\n";
	//cout<<shmY<<endl;
	int y  = atoi(shmY);
	y=y+1;
//	cout<<"After Increment = "<<y<<endl;
	char buffer[100];
	itoa(y,buffer,10);
//	cout<<":buffer  = "<<buffer<<endl;
	sprintf(shmX,buffer);
	cout<<"X= "<<shmX<<endl;
//	cout<<"c= "<<c<<endl;
	sleep(1);
	kill(c,SIGUSR2);


}
int main(int argc, char  *argv[])
{
    keyX = 5678;
    keyY = 5679;
    shmid1 = shmget(keyX, SHMSZ, IPC_CREAT | 0666);
    shmid2 = shmget(keyY, SHMSZ, IPC_CREAT | 0666);
    shmX =(char*) shmat(shmid1, NULL, 0);
	shmY = (char*)shmat(shmid2, NULL, 0);
	//cout<<"shmid's "<<shmid1<<" "<<shmid2<<endl;
	c = fork();
	signal(SIGUSR1,runnerread1);
	s[0]='1';
	s[1]='\0';
	sprintf(shmY,s);
	if(c>0)
	{
		wait(NULL);
	}
	else
	{
	//	cout<<"Child\n";
		execvp("./q22",NULL);
	}
	
	return 0;
}