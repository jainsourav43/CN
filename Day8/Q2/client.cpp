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
#include<bits/stdc++.h>
#define PORT 8080
using namespace std;
int main()
{
	struct sockaddr_in servaddr;
	int csfd;
	csfd = socket(AF_INET,SOCK_STREAM,0);
	if(csfd==-1)
	{
		cout<<"Error";
		exit(1);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port =htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	int status =connect(csfd,(struct sockaddr*) &servaddr,sizeof(servaddr));
	if(status ==-1)
	{
		cout<<"Error";
		exit(1);
	}
	while(1)
	{
	char hello[50];
	cout<<"Enter Message for server\n";
	cin>>hello;
	send(csfd,hello,strlen(hello),0);
	printf("Message sent  from client\n");
	char buffer[100];
	recv(csfd,buffer,100,0);
	cout<<"Message recieved from server  = "<<buffer<<endl;
	}
	return 0;
	
	
	
}
