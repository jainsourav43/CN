#include<bits/stdc++.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include<cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/un.h>
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
int main(int argc ,char* argv[])
{
	if(argc!=2)
	printf("\n usage ./a.out port_no");

	int sfd;
	struct sockaddr_in serv_addr;
	int port_no=atoi(argv[1]);

	bzero(&serv_addr,sizeof(serv_addr));

	if((sfd = socket(AF_INET , SOCK_STREAM , 0))==-1)
	perror("\n socket");
	else printf("\n socket created successfully\n");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr =  inet_addr("127.0.0.1");

	if(connect(sfd , (struct sockaddr *)&serv_addr , sizeof(serv_addr))==-1)
	perror("\n connect : ");
	else printf("\nconnect succesful %d ",sfd);
	int t,l;
	cout<<"Enter the Value of t and l";
	cin>>t>>l;
	char buf[50];
	itoa(t,buf);
	cout<<"Sending t\n";
	send(sfd,buf,strlen(buf),0);
	cout<<"t Sent\n";
	cout<<"Sending l\n";
	bzero(buf,50);
	sleep(1);
	itoa(l,buf); 	
	send(sfd,buf,strlen(buf),0);
	cout<<"l sent\n";
	
	sleep(10);
	recv(sfd,buf,50,0);
	cout<<"Service from X  = "<<buf<<endl;
	
	
	
	
	
}
