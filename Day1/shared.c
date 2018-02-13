#include<bits/stdc++.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/shm.h>
#define SEM1 "/sem1"
#define SEM2 "/sem2"

using namespace std;

int num = 0;

void writeX(char* ch)
{
	string str = (string)itoa(num++,ch,10); 
	str += "X";
	ch = str.to_c();
}

void readX(char* ch)
{
	cout<<*ch<<endl;
}

void writeY(char* ch)
{
	string str = (string)itoa(num++,ch,10); 
	str += "Y";
	ch = str.to_c();
}

void readY(char* ch)
{
	cout<<*ch<<endl;
}

int main()
{
	sem_t *sem1 = sem_open(SEM1, O_CREAT, 0644, 1);
	sem_t *sem2 = sem_open(SEM2, O_CREAT, 0644, 0);
	
	int xid,yid;
	key_t keyX = 5678;
	key_t keyY = 5679;
	char* shmx;
	char* shmY;

	xid = shmget(keyX,MAXSIZE,IPC_CREAT | 0666);
	yid = shmget(keyY,MAXSIZE,IPC_CREAT | 0666);
	shmX = (char*)shmat(xid,NULL,0);
	shmY = (char*)shmat(yid,NULL,0);
	
	int c = fork();
	if(c>0)
	{
		//P1
		while(true)
		{
			sem_wait(&sem1);
			readX(shmX);
			writeY(shmY);
			wait(1);
			sem_post(&sem2);
		}
	}
	else
	{
		//P2
		while(true)
		{
			sem_wait(&sem2);
			readY(shmY);
			writeX(shmX);
			wait(1);
			sem_post(&sem1);
		}
	}

	return 0;	
}