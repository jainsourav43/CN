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
using namespace std;
void itoa(int num,char* buf)
{
	int i=0;char temp[50];
	while(num!=0)
	{
		int x=num%10;
		temp[i++] = x+'0';
		num=num/10;
	}
	temp[i]='\0';
	int len=strlen(temp);
	i =len-1;int j=0;cout<<"Temp = "<<temp<<endl;
	while(i>=0)
	{
		buf[j] =temp[i];
		j++;i--;
	}
	buf[i]='\0';
	cout<<"Buf = "<<buf<<endl;
	
	
}
int main()
{
	int PORT;
   cout<<"ENter PORT\n";
	 cin>>PORT;
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
	int service;
	
		cout<<"Enter the Type of Service\n";
		cin>>service;
		char buffer[50];
		itoa(service,buffer);
		send(csfd,buffer,strlen(buffer),0);
		while(1)
		{
			recv(csfd,buffer,50,0);
			cout<<"received in Client  = "<<buffer<<endl;
		}
		
	return 0;
	
	
	
}
