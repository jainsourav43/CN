#include<stdio.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>-
#include<sys/select.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<bits/stdc++.h>
#include <arpa/inet.h>
using namespace std;
int main()
{
	int noofPort,i;
	cout<<"Enter the number of ports\n";
	cin>>noofPort;
	struct sockaddr_in addrport;
	int addrlen=0;
	int no_of_client=10;
	int PORT =9010;
	int sfd[noofPort];
	for(i=0;i<noofPort;i++,PORT++)
	{
		sfd[i]= socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(sfd[i]==-1)
		{
			cout<<"Error";
			exit(1);
		}
		addrport.sin_family =AF_INET;
		addrport.sin_addr.s_addr =INADDR_ANY;
		addrport.sin_port  =htons(PORT);
		int status = bind(sfd[i],(struct sockaddr*) &addrport,sizeof(addrport));
		if(status==-1)
		{
			cout<<"Error";
			exit(1);
		}
		status =listen(sfd[i],no_of_client);
		if(status==-1)
		{
			cout<<"Error";
			exit(1);
		}
	}
	fd_set rfd;
	struct timeval tv;
	tv.tv_sec = 15;
	tv.tv_usec =100;
	while(1)
	{
		for(i=0;i<noofPort;i++)
		{
			FD_SET(sfd[i],&rfd);
		}
		int retval = select(30,&rfd,NULL,NULL,&tv);
		//cout<<"retval"<<retval<<endl;
		int child=0;
		for(i=0;i<noofPort;i++)
		{
			//cout<<"Before i  ="<<i<<endl; 
			if(FD_ISSET(sfd[i],&rfd))
			{	
				cout<<"JUST I = "<<i <<endl;
				int nsfd = accept(sfd[i],(struct sockaddr*) &addrport,(socklen_t *)&addrlen);
				int c=fork();
				if(c>0)
				{
					close(nsfd);
				}
				else
				{
					cout<<"I = "<<i<<endl;
					char buffer[100];
					read(nsfd,buffer,100);
					cout<<"Message from Client = "<<buffer<<endl;
					send(nsfd,"Bye",3,0);
					cout<<"Message sent from server\n";
					child=1;
					
					break;
				}
			}
		}
		if(child==1)
		break;
	}
	
	
	
	return 0;
}
