#include<iostream>
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
#include<fstream>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


using namespace std;
int main(int argc,char* argv[])
{

	int PORT ;
	PORT =atoi(argv[1]);
	struct sockaddr_in servaddr,client_addr;
	int csfd;
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
	
	char portno[20];
	bzero(portno,20);
	
	cout<<"Enter the Port Number on which You will be accepting on\n";
	cin>>portno;
	
	

	int c=fork();
	if(c==0)
	{
		int myport  = atoi(portno);
		send(csfd,portno,strlen(portno)+1,0);
	
		cout<<"Port NUmber Sent\n";
	
		int clientfd  = socket(AF_INET,SOCK_STREAM,0);
	
	
		client_addr.sin_family =AF_INET;
		client_addr.sin_port= htons(myport);
		client_addr.sin_addr.s_addr =inet_addr("127.0.0.1");
	
		status =bind(clientfd,(struct sockaddr*) &client_addr,sizeof(client_addr));
	
		if(status==-1)
		{
			perror("bind");
		}
	
		status =listen(clientfd,10);
	
		if(status==-1)
		{
			perror("listen");
		}
	
	
	
	
	
	
	
	
		int serverfd;
	
		int length=0;
		serverfd =accept(clientfd,(struct sockaddr*) &client_addr,(socklen_t*)&length);
		if(serverfd==-1)
		{
			cout<<"Error in Accept on client Side\n";
			exit(0);
		}
		
		
		char filename[50];
		int filesize;
		char sizeoffile[10];
		
		cout<<"Enter the Name of File\n";
		cin>>filename;
		send(serverfd,filename,50,0);
		
		cout<<"Filename = "<<filename<<endl;
	
		recv(serverfd,sizeoffile,10,0);
	
		cout<<"File Size = "<<sizeoffile<<endl;
	
		filesize=atoi(sizeoffile);
	
		char data[1024];
		cout<<"File Size ="<<filesize<<endl;
		int fd=open(filename,O_WRONLY|O_CREAT);
		perror("open");
		int count=0;
		while(filesize>0)
		{
		
			bzero(data,1024);
			int noofBytes= recv(serverfd,data,1024,0);
			cout<<"No oF Bytes = "<<noofBytes<<"File Size  ="<<filesize<<endl;
			filesize-=noofBytes;
			write(fd,data,noofBytes);
			count++;
			if(count==50)
			break;
		} 
		
		close(fd);
	}
	else
	{
		wait(NULL);
	}
	

	
}
