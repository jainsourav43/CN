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
using namespace std;

int main()
{
	char* t1 = (char*)"/tmp/t1";
	char* t2 = (char*)"/tmp/t2";
	mkfifo(t1,0666);
	mkfifo(t2,0666);
	int fd[2];
	fd[0] = open(t1,O_WRONLY);
	fd[1] = open(t2,O_WRONLY);
	char n1[10],n2[10],op[2];
	int t;
	while(true)
	{
		cin>>t>>op>>n1>>n2;
		char buf[100];
		strcpy(buf,op);
		strcat(buf," ");
		strcat(buf,n1);
		strcat(buf," ");
		strcat(buf,n2);
		strcat(buf,"*");		//terminator
		if(t==1)
		{
			write(fd[0],buf,strlen(buf));
		}
		else if(t==2)
		{
			write(fd[1],buf,strlen(buf));
		}
	}
	
	close(fd[0]);
	close(fd[1]);
	
	return 0;
}
