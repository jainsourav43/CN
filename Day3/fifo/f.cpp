#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;
int main(int argc, char const *argv[])
{
	int c;
	char *newfifo = "/tmp/newfifo";
	mkfifo(newfifo,0666);
	c=fork();
	char s1[50],s2[50];
	int fd = open(newfifo,O_RDONLY);
	while(1)
	{
		read(fd,s1,strlen(s1));
		cout<<s1;
	}
	return 0;
}
