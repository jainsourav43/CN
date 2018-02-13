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
#include<sys/select.h>
using namespace std;
void runnerread1(int sig)
{
	FILE* fd = popen("./p4","w");
	char *s = "I Love You\n";
	int f1 = fileno(fd);
	dup2(f1,1);
	cout<<s<<endl;
	pclose(fd);
	dup2(1,f1);
	
}
int main(int argc, char const *argv[])
{
	char *newfifo = "/tmp/newfifo";
	mkfifo(newfifo,0666);
	int i,c;
	char *newfifo1 = "/tmp/newfifo1";
	mkfifo(newfifo1,0666);
	signal(SIGUSR1,runnerread1);
	string s[] ={"./p1","./p2","./p3"};
	string pipes[] = {"/tmp/newfifo","/tmp/newfifo1"};
	fd_set rfds,wfds;
	cout<<"Pid = "<<getpid()<<endl;
//	c=fork();

		if(1)
		{
			//sleep(1);
			int fd1,fd2;
			struct timeval tv;
			tv.tv_sec=10;
			tv.tv_usec =0;
		
			cout<<"PId Again"<<getpid()<<endl;
			fd1= open(newfifo,O_RDONLY);
			cout<<"After fd1\n";
			fd2 = open(newfifo1,O_RDONLY);	
			cout<<"Pipe Opened\n";
			char buffer[100];
			cout<<"Before While\n";
			while(1)
			{
			//	cout<<"Welcome\n";
				FD_ZERO(&rfds);
				FD_ZERO(&rfds);
				FD_SET(fd1,&rfds);
				FD_SET(fd2,&rfds);
				int retval = select(10,&rfds,NULL,NULL,&tv);
				//cout<<"RetVal ="<<retval<<endl;
				//cout<<"After = "<<FD_ISSET(fd1,&rfds)<<endl;
				if(FD_ISSET(fd1,&rfds))
				{
					bzero(buffer,100);
					//cout<<"First\n";
					read(fd1,buffer,50);
					printf(" %s ",buffer);
					sleep(2);
			
				}
				 if(FD_ISSET(fd2,&rfds))
				{
					bzero(buffer,100);
					//cout<<"Second-----------------------------------\n";
					read(fd2,buffer,4);
					printf(" %s ",buffer);
					sleep(2);
				}
				
			}
		}
		else
		{
			cout<<s[2]<<endl;
			execvp(s[2].c_str(),NULL);
			
		}
	
	
	
	
	
	return 0;
}
