#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<semaphore.h>
using namespace std;
#define SEM1 "/s1"
#define SEM2 "/s2"
int main(int argc, char  *argv[])
{
	int fd = open("f1.txt",O_RDONLY);
	sem_t *sem1 = sem_open(SEM1, O_CREAT, 0644, 1);
	sem_t *sem2 = sem_open(SEM2, O_CREAT, 0644, 0);
	sem_init(sem1,1,1);
	sem_init(sem2,1,0);
	int count=0;
	cout.flush();
	int c;
	c=fork();
	int std_in = dup(0);
	dup2(fd,0);
	if(c>0)
	{
		while(1)
		{
			int val;
			sem_getvalue(sem1,&val);
			sem_wait(sem1);
			char *buffer,*line;
			line = new char[1024];
			buffer = new char[1024];
			dup2(0,fd);
			if(read(fd, buffer, 1)==0)
			{
				break;
			}
			int i = 0;
	    	while(*buffer != '\n' && *buffer != '\0')
		    {
		        line[i] = *buffer;
		        ++i;
		        *buffer++;
		        if(read(fd, buffer, 1)==0)
				{
					break;
				}//Assuming i < 1024 and *buffer != NULL
	    	}
	    	cout<<"IN filemulti = "<<line<<endl;
	    	cout.flush();
	    	dup2(fd,0);
	    	sem_post(sem2);
    	}

	}
	else
	{
		char* args[] = {"./p2",NULL};
		execvp(args[0],args);

	}
	close(fd);
	dup2(std_in,0);
	return 0;
}