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

using namespace std;
int main()
{
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addrport ;
	int PORT;
	cout<<"Enter PORT\n";
	cin>>PORT;
	addrport.sin_family =AF_INET;
	addrport.sin_port  =htons(PORT);
	addrport.sin_addr.s_addr = INADDR_ANY;
	int status = bind(sfd,(struct sockaddr*) &addrport,sizeof(addrport));
	
	if(status==-1)
	{
		cout<<"Error";
		exit(1);
	}
	int no_of_client=10,addrlen;
	status =listen(sfd,no_of_client);
	int running[5],i;
	for(i=0;i<5;i++)
	{
		running[i] = 0;
	}
	
	if(status==-1)
	{
		cout<<"Error";
		exit(1);
	}
	pid_t pids[5];
	for(; ; )
	{
		char buf[50];
		int nsfd = accept(sfd,(struct sockaddr*) &addrport,(socklen_t *)&addrlen);
		recv(nsfd,buf,50,0);
		int service=atoi(buf);
		cout<<"SErvice number from Client "<<service<<endl;
		if(running[service]==0)
		{
			int c=fork();
			cout<<"c = "<<c<<endl;
			if(c>0)
			{
				pids[service]= c;
			}
			else
			{
				dup2(nsfd,0);
				cout<<"New Service Exec \n";
				execvp("./D1",NULL);
			}
			running[service]= 1;
		} 
		else
		{
			dup2(nsfd,0);
			cout<<"Before KILL\n";
			cout<<"PId = "<<pids[service]<<endl;
			kill(pids[service],SIGUSR1);
		}
		
	}
	
	
	
}
