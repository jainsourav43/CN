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
#define PORT 8080
using namespace std;
int main()
{
	struct sockaddr_in addrport,addrport1;
	struct sockaddr_in client_address;
	int addrlen=0;
	int no_of_client=10;
	int sfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sfd==-1)
	{
		cout<<"Error";
		exit(1);
	}
	addrport.sin_family =AF_INET;
	addrport.sin_port  =htons(PORT);
	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	int status = bind(sfd,(struct sockaddr*) &addrport,sizeof(addrport));
	if(status==-1)
	{
		cout<<"Error";
		exit(1);
	}
	status =listen(sfd,no_of_client);
	if(status==-1)
	{
		cout<<"Error";
		exit(1);
	}
	int fd2;
	for(; ; )
	{
		int nsfd = accept(sfd,(struct sockaddr*) &addrport,(socklen_t *)&addrlen);

	    char *ip  = inet_ntoa(addrport.sin_addr);
		cout<<"ip = "<<ip<<endl; 
	    cout<<"nsfd= "<<nsfd<<endl;
		int c=fork();
		dup2(nsfd,fd2);
		if(c>0)
		{
			close(nsfd);
		}
		else
		{
			close(sfd);
			char buffer[100];
			//dup2(0,nsfd);
			read(fd2,buffer,100);
			cout<<"Message from Client = "<<buffer<<endl;
			send(fd2,"Bye",3,0);
			cout<<"Message sent from server\n";
			break;
		}
	}

	return 0;
}
