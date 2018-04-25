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
	struct sockaddr_in client_address,server_address;
	
	
	int csfd = socket(AF_INET,SOCK_STREAM,0);
	
	
	server_address.sin_family =AF_INET;
	server_address.sin_port=htons(PORT);
	server_address.sin_addr.s_addr =inet_addr("127.0.0.1");
	
	int status =bind(csfd,(struct sockaddr*) &server_address,sizeof(server_address));
	
	if(status==-1)
	{
		perror("bind");
	}
	
	
	status =listen(csfd,10);
	
	if(status==-1)
	{
		perror("listen");
	}
	
	int length=0;
	
	int nsfd = accept(csfd,(struct sockaddr*) &client_address,(socklen_t*)&length);
	
	
	//cout<<"Client Family  = "<<client_address.sin_family<<endl;
	//cout<<"Client Family  = "<<client_address.sin_addr.s_addr<<endl;
	
	if(nsfd==-1)
	{
		perror("accept");
	}
	
	
	cout<<"recieving Port\n";
	char port_no[50];
	recv(nsfd,port_no,50,0);
	
	cout<<"Port Number = "<<port_no<<endl;
	
	int clientport=atoi(port_no);
	
	
	client_address.sin_addr.s_addr =inet_addr("127.0.0.1");
	client_address.sin_family =AF_INET;
	client_address.sin_port =htons(clientport);
	
	
	cout<<"Cout connecting To the Client to send the file\n";
	
	
	
	
	int c=fork();
	if(c==0)
	{
		
		int clientfd=socket(AF_INET,SOCK_STREAM,0);
		if(status==-1)
		{
			perror("socket");
		}
		cout<<"client Family  = "<<client_address.sin_family<<endl;
		cout<<"client Family  = "<<inet_ntoa(client_address.sin_addr)<<endl;
		status = connect(clientfd,(struct sockaddr*)&client_address,sizeof(client_address));
		if(status==-1)
		{
			perror("connect");
		}
		
		//cout<<"Sending File name\n";

		//strcpy(filename,"CloneLinkedlist1.txt");
		//strcat(filename,"\0\n");
		//send(clientfd,filename,strlen(filename)+1,0);
		
		
		struct stat filestat;
		char filename[100];
		int bytesrecv= recv(clientfd,filename,100,0);
		cout<<"File Name Received from client  = "<<filename<<endl;
		int fd =open(filename,O_RDONLY);
		int check =fstat(fd,&filestat);

		int filesize =filestat.st_size;
		string s =to_string(filesize);
	
		char sizeoffile[10];
		strcpy(sizeoffile,s.c_str());
		strcat(sizeoffile,"\0\n");
		
		cout<<"File Name Sent and Sending Length of File = "<<sizeoffile<<endl;;
		send(clientfd,sizeoffile,strlen(sizeoffile)+1,0);
		cout<<"Size Sent\n";
		
		char data[1025];
		while(filesize>0)
		{
			bzero(data,1025);
			int noOfBytes =read(fd,data,1024);
			cout<<"Sending "<<filesize<<"  NoOFBytes = "<<noOfBytes<<endl;
			send(clientfd,data,noOfBytes,0);
			filesize-=noOfBytes;
		}
	}
	else
	{
	wait(NULL);
	}
	
	
	

	return 0;
}
