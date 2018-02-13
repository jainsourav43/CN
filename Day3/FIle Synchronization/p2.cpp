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
		sem_t *sem1 = sem_open(SEM1,0);
        sem_t *sem2 = sem_open(SEM2,0);
        int count=0;
        cout.flush();
		while(1)
		{
			sem_wait(sem2);
			// cout<<"WAIT IN P@2\n";
			char *buffer,*line;
			buffer  = new char[1024];
			line = new char[1024];
			
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
				} //Assuming i < 1024 and *buffer != NULL
	    	}
	    	cout<<"in p2 = "<< line<<endl;
	    	cout.flush();
	    	dup2(fd,0);
	    	sem_post(sem1);
    	}
    	close(fd);
	return 0;
}