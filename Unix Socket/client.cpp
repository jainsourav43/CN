#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <sys/un.h>

#define path "./socket"

using namespace std;

int main()
{
	int sockfd;
	sockfd =socket(AF_LOCAL,SOCK_STREAM,0);
	sockaddr_un clientaddr,servaddr;
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sun_family=AF_LOCAL;
	strcpy(servaddr.sun_path,path);
	int status =connect(sockfd,(sockaddr*)&servaddr,sizeof(servaddr));
	if(status<0)
	{
		cout<<"Conect Error";
		exit(0);
	}
	
	 int rc;
	 msghdr childmsg;
	 memset(&childmsg,0,sizeof(childmsg));
	 char cmsgbuf[CMSG_SPACE(sizeof(int))];
	 childmsg.msg_control=cmsgbuf;
	 childmsg.msg_controllen =sizeof(cmsgbuf);
	 printf("Waiting on recvmsg\n");
	 
	 
	 
	 rc=recvmsg(sockfd,&childmsg,0);
	 struct cmsghdr *cmsg=CMSG_FIRSTHDR(&childmsg);
	 
	 
	 if (cmsg == NULL) {
		    printf("cmsg is null\n");
		    exit(0);
	  }
	  if(cmsg -> cmsg_type != SCM_RIGHTS)
	  {
		 cout<<"\nno SCM_RIGHTS\n";
		 exit(0);
	  }
	  
	  int filefd ;
	  memcpy(&filefd,CMSG_DATA(cmsg),sizeof(filefd));
	  printf("Received descriptor = %d\n", filefd);
	  
	  
/*	  char buffer[100];
	  read(filefd,buffer,4);
	  cout<<"Buffer = "<<buffer<<endl;

	  */
	  
	  for(; ; )
	  {
	  		socklen_t len =sizeof(clientaddr);
	  		int connfd=accept(filefd,(sockaddr*)&clientaddr,&len);
	  		cout<<"Accepted in Client  = "<<connfd<<endl;

	  }
	 
	  	return 0;
}
