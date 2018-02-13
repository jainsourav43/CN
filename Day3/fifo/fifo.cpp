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
	if(c>0)
	{
		int fd = open(newfifo,O_RDONLY);
		//cout<<"fd  ="<<fd<<endl;
		char s[50];
		read(fd,s,50);
		printf("%s\n",s );
	}
	else
	{

		int fd = open(newfifo,O_WRONLY);
		printf("Enter the String \n");
		char s[50];
		fgets(s,50,stdin);
		write(fd,s,strlen(s));

	}
	return 0;
}