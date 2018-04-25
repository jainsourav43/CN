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

using namespace std;
int main(int argc,char* argv[])
{
	struct sockaddr_in addrport,addrport1;
	struct sockaddr_in client_address;
	int PORT;
	PORT=atoi(argv[1]);
	int addrlen=0;
	int no_of_client=10;
	int sfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sfd==-1)
	{
		perror("socket");
		exit(1);
	}
	addrport.sin_family =AF_INET;
	addrport.sin_port  =htons(PORT);
	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	int status = bind(sfd,(struct sockaddr*) &addrport,sizeof(addrport));
	if(status==-1)
	{
		perror("bind");
		exit(1);
	}
	status =listen(sfd,no_of_client);
	if(status==-1)
	{
		perror("listen");
		exit(1);
	}
	
	
	int nsfd = accept(sfd,(struct sockaddr*) &addrport,(socklen_t *)&addrlen);
	if(nsfd==-1)
	{
		perror("accept\n");
	}
    char buffer[100];
	int noofBytes=recv(nsfd,buffer,100,0);
	cout<<"No of Bytes = "<<noofBytes<<"IP5 =  "<<buffer<<endl;
	
	struct sockaddr_in addressIP5;
	
	addressIP5.sin_family=AF_INET;
	addressIP5.sin_addr.s_addr=inet_addr("172.30.102.125");
	bzero(buffer,100);
	
	 noofBytes=recv(nsfd,buffer,100,0);
	cout<<"No of Bytes = "<<noofBytes<<"Filename =  "<<buffer<<endl;
	

	
	int fd=open(buffer,O_RDONLY);
	perror("open");
	cout<<"Creating Raw Socket\n";
	
		bzero(buffer,100);
	int rsfd =socket(AF_INET,SOCK_RAW,91);
	connect(rsfd,(struct sockaddr*)&addressIP5,sizeof(addressIP5));
	write(rsfd,"Hello",5);
	if(rsfd==-1)
	{
		perror("socket");
		exit(0);
	}
	
	cout<<"executing s2.c\n";
	
	int c=fork();
	
	if(c==-1)
	{
		perror("fork");
		exit(1);
	}
	else if(c==0)
	{
		dup2(fd,0);
		dup2(rsfd,1);
		execvp("./s2",NULL);
	}
	else
	{
		//Do Nothing	
	}

	
	
	



	return 0;
}
