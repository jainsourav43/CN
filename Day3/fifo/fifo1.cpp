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
	char s1[50],s2[50];
	while(1)
	{
		int fd = open(newfifo,O_WRONLY);
		printf("Enter the String \n");
		fgets(s1,50,stdin);
		write(fd,s1,strlen(s1));
		close(fd);

	}
	return 0;
}