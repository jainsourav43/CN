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
#include<sys/un.h>
using namespace std;
#define ADDRESS     "mysocket"


int recv_fd(int socket)
 {
  int sent_fd, available_ancillary_element_buffer_space;
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

  /* start clean */
  memset(&socket_message, 0, sizeof(struct msghdr));
  memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

  /* setup a place to fill in message contents */
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

  if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
   return -1;

  if(message_buffer[0] != 'F')
  {
   /* this did not originate from the above function */
   return -1;
  }

  if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
  {
   /* we did not provide enough space for the ancillary element array */
   return -1;
  }

  /* iterate ancillary elements */
   for(control_message = CMSG_FIRSTHDR(&socket_message);
       control_message != NULL;
       control_message = CMSG_NXTHDR(&socket_message, control_message))
  {
   if( (control_message->cmsg_level == SOL_SOCKET) &&
       (control_message->cmsg_type == SCM_RIGHTS) )
   {
    sent_fd = *((int *) CMSG_DATA(control_message));
    return sent_fd;
   }
  }

  return -1;
 }



int main(int argc,char* argv[])
{
	struct sockaddr_in addrport,addrport1;
	struct sockaddr_in client_address;
	int PORT=atoi(argv[1]);
	cout<<"PORt = "<<PORT<<endl;
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
	int enable = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
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
	int fd2;
	
	
	
		int nsfd = accept(sfd,(struct sockaddr*) &addrport,(socklen_t *)&addrlen);
		char buffer[50];
		int noOfBytes = recv(nsfd,buffer,50,0);
		cout<<"NoOfBytes = "<<noOfBytes<<endl;
		cout<<"From Client  ="<<buffer<<endl;
		bzero(buffer,50);
		//close(nsfd);
		close(sfd);
		
		
		
		
		int usfd;
	struct sockaddr_un userv_addr;
  	int userv_len,ucli_len;

  	usfd = socket(AF_UNIX, SOCK_STREAM, 0);

  	if(usfd==-1)
  	perror("\nsocket  ");

  	bzero(&userv_addr,sizeof(userv_addr));
  	userv_addr.sun_family = AF_UNIX;
   	strcpy(userv_addr.sun_path, ADDRESS);

	userv_len = sizeof(userv_addr);
	cout<<"hi\n";
	sleep(4);
	connect(usfd,(struct sockaddr *)&userv_addr,userv_len);
	perror("connect");
	
	int receivedfd=recv_fd(usfd);
	cout<<"fd = "<<receivedfd<<endl;
	
	
	
	
	
	
	
	
		
		
		
		

	return 0;
}
