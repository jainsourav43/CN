#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<semaphore.h>

using namespace std;
int main(int argc, char  *argv[])
{
	FILE* fd = popen("./popen2","r");
	//char *s = "I Love You\n";
	char s[50];
	//fscanf(fd,"%s",s);
	fgets(s,50,fd);
	//int f1 = fileno(fd);
	//dup2(f1,1);
	cout<<s<<endl;
	pclose(fd);
	return 0;
}
