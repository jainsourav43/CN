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
#include<fcntl.h>
using namespace std;
int main(int argc,char* argv[])
{
	struct sockaddr_in servaddr;
	int csfd;
	int PORT;
	PORT = atoi(argv[1]);
	csfd = socket(AF_INET,SOCK_STREAM,0);
	if(csfd==-1)
	{
		cout<<"Error";
		exit(1);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port =htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int status =connect(csfd,(struct sockaddr*) &servaddr,sizeof(servaddr));
	if(status ==-1)
	{
		cout<<"Error";
		exit(1);
	}
	char ipaddress[20];
	cout<<"Enter IP5 you want ot send to Server\n";
	cin>>ipaddress;
	send(csfd,ipaddress,strlen(ipaddress)+1,0);
	cout<<"IP5 Sent\n";
	cout<<"Enter the File Name you want to send to the Server\n";
	char filename[50];
	cin>>filename;
	send(csfd,filename,strlen(filename)+1,0);
	cout<<"File Name sent";
	return 0;
	
	
	
}
