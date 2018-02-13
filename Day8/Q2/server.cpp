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
#include<pthread.h>
#define PORT 8080
using namespace std;
void* serverfunc(void* p)
{
	int nsfd = *(int* )p;
	cout<<"nsfd = "<<nsfd<<endl;
	char buffer[100];
	//dup2(0,nsfd);
	read(nsfd,buffer,100);
	cout<<"Message from Client = "<<buffer<<endl;
	send(nsfd,"Bye",3,0);
	cout<<"Message sent from server\n";
	
}
int main()
{
	struct sockaddr_in addrport;
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
	addrport.sin_addr.s_addr = INADDR_ANY;
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
	for(; ; )
	{
		int nsfd = accept(sfd,(struct sockaddr*) &addrport,(socklen_t *)&addrlen);
		pthread_t tid;
		pthread_create(&tid,NULL,serverfunc,(void *) &nsfd);
		pthread_join(tid,NULL);
		
	}

	return 0;
}
