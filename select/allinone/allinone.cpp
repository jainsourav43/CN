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
#define GSIZE 1000
using namespace std;

struct timeval tv;
int ret;
char* newfifo;
int fd[2];
int ffd;
char buf[105];
char gbuf[GSIZE];
fd_set rfds;

void sigf(int sigid)
{	
	FILE *fp=popen("./writer","w");
	int desp=fileno(fp);
	cout<<"len: "<<strlen(gbuf)<<endl;
	write(desp,gbuf,strlen(gbuf));
	close(desp);
	
	strcpy(gbuf,"");	
	
	//RESET ALL SELECT PARAMETERS
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	//CREATE FD_SET FOR ALL FDs
	FD_ZERO(&rfds);
    //SET ALL RELEVANT FDs
    FD_SET(0,&rfds);
    FD_SET(fd[RDEND],&rfds);
	FD_SET(ffd,&rfds);
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
			//MODIFY SIGNAL SIGUSR1
			signal(SIGUSR1,sigf);	
			int sid = shmget(5678,10,IPC_CREAT | 0666);
			char* shm = (char*)shmat(sid,NULL,0);
			char str[10];
			itoa(str,getpid());
			sprintf(shm,str);
			//CREATE FIFO
			newfifo = (char*)"/tmp/aiofifo";
			mkfifo(newfifo,0666);
			ffd = open(newfifo,O_RDONLY);
			bzero(gbuf,GSIZE);
		    while(true)
		    {
				tv.tv_sec = 5;
				tv.tv_usec = 0;
				//CREATE FD_SET FOR ALL FDs
				FD_ZERO(&rfds);
			    //SET ALL RELEVANT FDs
			    FD_SET(0,&rfds);
			    FD_SET(fd[RDEND],&rfds);
				FD_SET(ffd,&rfds);
				
				ret = select(10, &rfds, NULL, NULL, &tv);
				if(strlen(gbuf) >= GSIZE)
					continue;
				if(ret==0)
				{
					cout<<"TIMEOUT!"<<endl;
					close(ffd);
					close(fd[RDEND]);
					return 0;
				}
				else if(ret > 0)
				{
					if(FD_ISSET(0,&rfds))
					{
						strcat(gbuf,"KEYBOARD\n");
						bzero(buf,105);
						int r = read(0,buf,100);
						strcat(gbuf,buf);
					}
					if(FD_ISSET(fd[RDEND],&rfds))
					{
						strcat(gbuf,"PIPE\n");
						bzero(buf,105);
						int r = read(fd[RDEND],buf,100);
						strcat(gbuf,buf);
					}
					if(FD_ISSET(ffd,&rfds))
					{
						strcat(gbuf,"FIFO\n");
						bzero(buf,105);
						int r = read(ffd,buf,100);
						strcat(gbuf,buf);
					}
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
				sleep(3);				
			}
		}
		else
		{
			cout<<"ERROR: Fork failed!";
			return -1;
		}
	}
}
