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
#include<stdio.h> //for printf
#include<string.h> //memset
#include<sys/socket.h>    //for socket ofcourse
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include <arpa/inet.h>
using namespace std;
struct sockaddr_in source,dest;
int log_txt =1;
uint16_t ip_header( char* buffer,int buflen)
{
	struct iphdr *ip = (struct iphdr*)(buffer);

	int iphdrlen =ip->ihl*4;
	
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = ip->saddr;  
	if(strcmp(inet_ntoa(source.sin_addr),"1.2.3.4")==0)   
	return -1;
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = ip->daddr;     

	dprintf(log_txt , "\nIP Header\n");

	dprintf(log_txt , "\t|-Version              : %d\n",(unsigned int)ip->version);
	dprintf(log_txt , "\t|-Internet Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
	dprintf(log_txt , "\t|-Type Of Service   : %d\n",(unsigned int)ip->tos);
	dprintf(log_txt , "\t|-Total Length      : %d  Bytes\n",ntohs(ip->tot_len));
	dprintf(log_txt , "\t|-Identification    : %d\n",ntohs(ip->id));
	dprintf(log_txt , "\t|-Time To Live	    : %d\n",(unsigned int)ip->ttl);
	dprintf(log_txt , "\t|-Protocol 	    : %d\n",(unsigned int)ip->protocol);
	dprintf(log_txt , "\t|-Header Checksum   : %d\n",ntohs(ip->check));
	dprintf(log_txt , "\t|-Source IP         : %s\n", inet_ntoa(source.sin_addr));
	dprintf(log_txt , "\t|-Destination IP    : %s\n",inet_ntoa(dest.sin_addr));

	
	return 0;
	


	
}

int main(int argc,char* argv[])
{

	struct sockaddr_in addrport,addrport1,serveraddr;
	struct sockaddr_in client_address;
	int addrlen=0;
	int no_of_client=10;
	int sfd = socket(AF_INET,SOCK_RAW,91);
	if(sfd==-1)
	{
		perror("socket");
		exit(1);
	}
	char buffer[65535];
	struct sockaddr_in clientaddr;
	clientaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	clientaddr.sin_family=AF_INET;
	strcpy(buffer,"Inquiry about services");
	int noOfBytes =sendto(sfd,buffer,strlen(buffer)+1,0,(sockaddr*)&clientaddr,sizeof(clientaddr));
	
	
	perror("sendto");
	cout<<"Sent = "<<noOfBytes<<endl;
	bzero(buffer,65535);
	socklen_t length;
	cout<<"Buffer Before ="<<buffer<<endl;
    noOfBytes= recvfrom(sfd,buffer,65535,0,(struct sockaddr*)&serveraddr,&length);
    perror("recvfrom");
    cout<<"No of Bytes ="<<noOfBytes<<endl;
	int serverPort =ip_header(buffer,noOfBytes);
	cout<<"IP = "<<inet_ntoa(serveraddr.sin_addr)<<endl;
	cout<<"Server PORT = "<<serverPort<<endl;
	cout<<"Buffer = "<<(char*)(buffer+sizeof(struct iphdr))<<endl;
	
	bzero(buffer,65535);
	noOfBytes= recvfrom(sfd,buffer,65535,0,(struct sockaddr*)&serveraddr,&length);
	cout<<"port = "<<ntohs(serveraddr.sin_port)<<endl;
	serverPort =atoi(buffer+sizeof(iphdr));
	cout<<"Server PORT = "<<serverPort<<endl;
	cout<<"Buffer = "<<(char*)(buffer+sizeof(struct iphdr))<<endl;
	
	bzero(buffer,65535);
	noOfBytes= recvfrom(sfd,buffer,65535,0,(struct sockaddr*)&serveraddr,&length);
	cout<<"port = "<<ntohs(serveraddr.sin_port)<<endl;
	serverPort =ip_header(buffer,noOfBytes);
	cout<<"Server PORT = "<<serverPort<<endl;
	cout<<"Buffer = "<<(char*)(buffer+sizeof(struct iphdr))<<endl;
	
	
	
	
	
	return 0;
}
