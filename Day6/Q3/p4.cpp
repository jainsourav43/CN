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
using namespace std;
int main()
{
	//char *newfifo2 = "/tmp/newfifo2";
	//mkfifo(newfifo2,0666);
	//int fd = open(newfifo2,O_RDONLY);
	
		char s[50];
		fgets(s,50,stdin);
		cout<<strlen(s)<<endl;
		cout<<s<<endl;cout<<"Output in p4= "<<s<<endl;
		sleep(2);

}
