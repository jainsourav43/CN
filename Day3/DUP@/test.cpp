#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;
int main()
{
	int fd = open("f2.txt",O_WRONLY);
	dup2(fd,1);
	cout<<fd<<endl;
	cout<<"Shriyansh madarjaat hai\n";
	return 0;
}