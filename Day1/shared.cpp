#include<bits/stdc++.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<cstdlib>
#include<fcntl.h>

#define SEM1 "/sem1"
#define SEM2 "/sem2"
#define MAXSIZE 30

using namespace std;

int num = 0;
char* strx = "abcde";
char* rstrx = "edcba";
char* stry = "klmno";
char* rstry = "onmlk";

void writeX(int num,char* ch)
{
	cout<<"writing X"<<endl;
	if(ch==strx)
	{
		memcpy(ch,rstrx,5);
	}
	else
	{
		memcpy(ch,strx,5);
	}
}

void readX(char* ch)
{
	cout<<"reading X"<<endl;
	cout<<ch<<endl;
}

void writeY(int num,char* ch)
{
	cout<<"writing Y"<<endl;
	if(ch==stry)
	{
		memcpy(ch,rstry,5);
	}
	else
	{
		memcpy(ch,stry,5);	
	}
}

void readY(char* ch)
{
	cout<<"reading Y"<<endl;
	cout<<ch<<endl;
}

int main()
{
	sem_t *sem1 = sem_open(SEM1, O_CREAT, 0644, 0);
	sem_t *sem2 = sem_open(SEM2, O_CREAT, 0644, 0);
	
	int xid,yid;
	key_t keyX = 5678;
	key_t keyY = 5679;
	char* shmX;
	char* shmY;

	sem_init(sem1,1,1);
	sem_init(sem2,1,0);

	xid = shmget(keyX,MAXSIZE,IPC_CREAT | 0666);
	yid = shmget(keyY,MAXSIZE,IPC_CREAT | 0666);
	shmX = (char*)shmat(xid,NULL,0);
	shmY = (char*)shmat(yid,NULL,0);

	shmX = strx;
	shmY = stry;

	int val;
	int c = fork();
	if(c>0)
	{
		//P1
		while(true)
		{
			sem_wait(sem1);
			readX(shmX);
			writeY(yid,shmY);
			sleep(1);
			sem_post(sem2);
		}
	}
	else
	{
		//P2
		while(true)
		{
			sem_wait(sem2);
			readY(shmY);
			writeX(xid,shmX);
			sleep(1);
			sem_post(sem1);
		}
	}

	return 0;	
}