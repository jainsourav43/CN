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
#include<bits/stdc++.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<fcntl.h>
#include<linux/ip.h>
using namespace std;
int main()
{
		int rsfd =socket(AF_INET,SOCK_RAW,91);
		perror("socket");
		struct sockaddr_in client;
		char buffer[100];
		socklen_t length;
		while(1)
		{
		int noofBytes = recvfrom(rsfd,buffer,100,0,(struct sockaddr*)&client,&length);
		if(noofBytes==-1) continue;
		cout<<"NofoBytes ="<<noofBytes<<endl;
		cout<<(char*)(buffer+sizeof(iphdr))<<endl;
		}
}
