#include<bits/stdc++.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/un.h>
#define path "./socket"
using namespace std;
int csfd[10];
int noofClient=0;
void handler(int sig)
{
	   int sock;
	   int sock_temp;
	   sockaddr_un cliaddr,servaddr;
	   sock_temp = socket(AF_UNIX,SOCK_STREAM,0);
	   bzero(&servaddr,sizeof(servaddr));
	   servaddr.sun_family = AF_UNIX;
	   strcpy(servaddr.sun_path,path);
		cout<<"Waiting on connect\n";
	   if(connect(sock_temp,(sockaddr*)&servaddr,sizeof(servaddr))<0)
	   {
		cout<<"Connection Failed\n";
	   }
	
	
     int rc;
	  msghdr child_msg;
	  memset(&child_msg,   0, sizeof(child_msg));
	  char cmsgbuf[CMSG_SPACE(sizeof(int))];
	  child_msg.msg_control = cmsgbuf; // make place for the ancillary message to be received
	  child_msg.msg_controllen = sizeof(cmsgbuf);

	  printf("Waiting on recvmsg\n");
	  rc = recvmsg(sock_temp, &child_msg, 0);
	  cout<<"rc = "<<rc<<endl;
	  struct cmsghdr *cmsg = CMSG_FIRSTHDR(&child_msg);
	  if (cmsg == NULL) {
		    printf("cmsg is null\n");
		    exit(0);
	  }
	  if(cmsg -> cmsg_type != SCM_RIGHTS)
	  {
		 cout<<"\nno SCM_RIGHTS\n";
		// exit(0);
	  }
	  int pass_sd;
	  memcpy(&sock, CMSG_DATA(cmsg), sizeof(sock));
	  csfd[noofClient++]=sock;
	  
}

int main()
{
	signal(SIGUSR1,handler);
	int i;
	dup2(1,csfd[noofClient++]);
	for(i=0;i<noofClient;i++)
	{
		dup2(1,csfd[i]);
		cout<<"Sending from D3"<<endl;
	}
	return 0;
	
}

