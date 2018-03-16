#include <bits/stdc++.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string>

#define RDEND 0
#define WREND 1
using namespace std;

fd_set rfds;
struct timeval tv;
int ret;
char* newfifo;
int fd[2];
int ffd;
FILE* outf;

void sigf(int sigid)
{
	outf = popen("./writer","w");	
	if(FD_ISSET(0,&rfds))
	{
		char* keyb = (char*)"KEYBOARD\n";
		fprintf(outf,keyb,strlen(keyb));
		char buf[1000];
		bzero(buf,1000);
		int r;
		while(r = read(0,buf,1024))
		{
			fprintf(outf,buf,r);
			bzero(buf,1000);
		}
	}
	if(FD_ISSET(fd[RDEND],&rfds))
	{
		char* pipestr = (char*)"PIPE\n";
		fprintf(outf,pipestr,strlen(pipestr));
		char buf[1000];
		bzero(buf,1000);
		int r;
		while(r = read(fd[RDEND],buf,1024))
		{
			fprintf(outf,buf,r);
			bzero(buf,1000);
		}
	}
	if(FD_ISSET(ffd,&rfds))
	{
		char* fifostr = (char*)"FIFO\n";
		fprintf(outf,fifostr,strlen(fifostr));
		char buf[1000];
		bzero(buf,1000);
		int r;
		while(r = read(ffd,buf,1024))
		{
			fprintf(outf,buf,r);
			bzero(buf,1000);
		}
	}
}

void itoa(char* str,int n)
{
	int i = 0;
	while(n)
	{
		int rem = n%10;
		str[i] = (rem+'0');
		n /= 10;
		i++;
	}	
	str[i] = '\0';
	reverse(str,str+i);
}

int main()
{
	if(pipe(fd) < 0)
	{
		cout<<"ERROR: Pipe could not be created!";
		return -1;
	}
	else
	{
		int c = fork();
		if(c>0)
		{
			//PARENT
			close(fd[WREND]);	
			int sid = shmget(5678,10,IPC_CREAT | 0666);
			char* shm = (char*)shmat(sid,NULL,0);
			char str[10];
			itoa(str,getpid());
			sprintf(shm,str);
			//MODIFY SIGNAL SIGUSR1
			signal(SIGUSR1,sigf);
			//CREATE FIFO
			newfifo = (char*)"/tmp/aiofifo";
			mkfifo(newfifo,0666);
			ffd = open(newfifo,O_RDONLY);
			//CREATE FD_SET FOR ALL FDs
			
	        FD_ZERO(&rfds);
	        //SET ALL RELEVANT FDs
	        FD_SET(0,&rfds);
	        FD_SET(fd[RDEND],&rfds);
		    FD_SET(ffd,&rfds);
		    
		    tv.tv_sec = 5;
		    tv.tv_usec = 0;
		    
		    while(true)
		    {
				ret = select(3, &rfds, NULL, NULL, &tv);
				if(ret==0)
				{
					cout<<"TIMEOUT!"<<endl;
					return 0;
				}
			}		
		}
		else if(c==0)
		{
			//CHILD
			close(fd[RDEND]);
			char* cmsg = (char*)"This is Child Process\n";
			while(true)
			{
				write(fd[WREND],cmsg,strlen(cmsg));
				sleep(5);				
			}
		}
		else
		{
			cout<<"ERROR: Fork failed!";
			return -1;
		}
	}
}
