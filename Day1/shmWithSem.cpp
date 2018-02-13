#include<bits/stdc++.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<fcntl.h>
#include<stdlib.h>

#define SEM1 "/sem1"
#define SEM2 "/sem2"
#define MAXSIZE 10

using namespace std;

char* str = "a";

void writeX(char* ch1,char* ch2)
{
	cout<<"Writing X"<<endl;
	char* tmp = ch1;
	strcat(tmp,ch2);
	if(strlen(tmp)>20)
		tmp = "a";
	sprintf(ch1,tmp);
}

void writeY(char* ch1,char*ch2)
{
	cout<<"Writing Y"<<endl;
	char* tmp = ch1;
	strcat(tmp,ch2);
	if(strlen(tmp)>20)
		tmp = "a";
	sprintf(ch1,tmp);
}

char* readX(char* ch)
{
	cout<<"Reading X: ";
	cout<<ch<<" "<<strlen(ch)<<endl;
	return ch;
}

char* readY(char* ch)
{
	cout<<"Reading Y: ";
	cout<<ch<<" "<<strlen(ch)<<endl;
	return ch;
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

	xid = shmget(keyX,MAXSIZE,IPC_CREAT | 0666);
	yid = shmget(keyY,MAXSIZE,IPC_CREAT | 0666);
	shmX = (char*)shmat(xid,NULL,0);
	shmY = (char*)shmat(yid,NULL,0);
	
	sem_init(sem1,1,1);
	sem_init(sem2,1,0);
	
	sprintf(shmX,str);
	sprintf(shmY,str);
	
	int c = fork();
	if(c>0)
	{
		//P1
		while(true)
		{
			sem_wait(sem1);
			char* x = readX(shmX);
			writeY(shmY,x);	//write value of x to y
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
			char* y = readY(shmY);
			writeX(shmX,y);	//write value of y to x
			sleep(1);
			sem_post(sem1);
		}
	}

	return 0;	
}
