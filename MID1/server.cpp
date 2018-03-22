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
#define path "./socket"
using namespace std;	
int childpid[4];
void passTheFd(int sock)
{

	   int sockfd;
	  
	   sockaddr_un cliaddr,servaddr;
	   sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	   cout<<"Sockfd = "<<sockfd<<endl;
	   bzero(&servaddr,sizeof(servaddr));
	   servaddr.sun_family = AF_UNIX;
	   strcpy(servaddr.sun_path,path);
	   cout<<"Bind  = "<<bind(sockfd,(sockaddr*) &servaddr,sizeof(servaddr))<<endl;
	   cout<<"Listen = "<< listen(sockfd,5)<<endl;
	   socklen_t len = sizeof(cliaddr);
	   cout<<"Waiting on Accept\n";
		int connfd = accept(sockfd,(sockaddr*) &cliaddr,&len);
		if(connfd < 0)
		{
			cout<<"\nError in accept"<<connfd<<endl;
			exit(0);
		}
		cout<<"\nAccepted "<<connfd<<endl;
		iovec iov[1]={0};

		 const char *str = "file";
		 iov[0].iov_base = (void*) str;
		 iov[0].iov_len = strlen(str)+1;

		 msghdr parent_msg;
		 memset(&parent_msg,0,sizeof(parent_msg));
		 cmsghdr *cmsg;
		 char cmsgbuff[CMSG_SPACE(sizeof(sock))]={};
		 parent_msg.msg_name = NULL;
		 parent_msg.msg_namelen = 0;
		 parent_msg.msg_iov=iov;
		 parent_msg.msg_iovlen=1;
		 parent_msg.msg_control = cmsgbuff;
		 parent_msg.msg_controllen= sizeof(cmsgbuff);

		 cmsg = CMSG_FIRSTHDR(&parent_msg);
		 if(cmsg == NULL)
		 {
		   cout<<"\ncmsg is null in server side.\n";
		 }
		 cmsg->cmsg_level = SOL_SOCKET;
		 cmsg->cmsg_type = SCM_RIGHTS;
		 cmsg->cmsg_len = CMSG_LEN(sizeof(sock));
		 memcpy(CMSG_DATA(cmsg),&sock,sizeof(sock));
		 parent_msg.msg_controllen = cmsg->cmsg_len;
		int check;
		 check=sendmsg(connfd,&parent_msg,0);
		
}
int main()
{
	 int PORT;
	 cout<<"ENter PORT\n";
	 cin>>PORT;
	int sfd;
	int running[4];
	for(int i=0;i<4;i++)
	{
		running[i]=0;
	}
	int sdf;
	sfd =socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addrport,clientaddr;
	addrport.sin_family = AF_INET;
	addrport.sin_port = htons(PORT);
	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	int status  =bind(sfd,(struct sockaddr*) &addrport,sizeof addrport);
	if(status<0)
	{
		cout<<"Bind Error\n";
	}
	status =listen(sfd,10);
	if(status<0)
	{
		cout<<"Listen Error\n";
	}
	int addrlen=0;
	char buffer[10];
	string services[] = {"./D1","./D2","./D3","./D4"};
	while(1)
	{
		int nsfd =accept(sfd,(sockaddr*)&clientaddr,(socklen_t*)&addrlen);
		if(nsfd<0)
		{
			cout<<"Accept Error\n";
			exit(0);
		}
		recv(nsfd,buffer,10,0);
		cout<<"Type of Service = "<<buffer<<endl;
		int service=atoi(buffer);
		if(running[service-1]==0)
		{
			running[service-1]=1;
			childpid[service-1]=fork();
		    cout<<"Child pidin = "<<childpid[service-1]<<endl;
			if(childpid[service-1]==0)
			{
			//cout<<"Starting Service  "<<c_str(services[service-1])<<endl;
			dup2(nsfd,1);
			execvp(services[service-1].c_str(),NULL);
			}

			
		}
		else
		{
			cout<<"Child pid = "<<childpid[service-1]<<endl;
			kill(childpid[service-1],SIGUSR1);
			passTheFd(nsfd);
			cout<<"Passed The Fd";
		}
		
	}
	
	
	
	
	
}

