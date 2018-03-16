#include<bits/stdc++.h>
#include<stdio.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <arpa/inet.h>
#define PORT 8080
using namespace std;
int fds[5],count1=0;
void signalhandler(int sig)
{
	cout<<"Before Signal Handler\n";
	dup2(0,fds[count1++]);
	for(int i=0;i<count1;i++)
	{
		send(fds[i],"Service from D1",16,0);
	}
	
}

void singleclientfun()
{
	send(fds[0],"Service from D1",16,0);
	
}
int main()
{
	cout<<"Entering IN D1"<<endl;
	signal(SIGUSR1,signalhandler);
	dup2(0,fds[count1++]);
	singleclientfun();
	
}
