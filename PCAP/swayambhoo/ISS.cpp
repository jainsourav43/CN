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
#include<pthread.h>
#include<sys/un.h>
#define ADDRESS     "mysocket"
using namespace std;


int send_fd(int socket, int fd_to_send)
 {
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket, &socket_message, 0);
 }
 
 
 
void connectandpass(int port)
{
	struct sockaddr_in newclient;

	cout<<"New Port ="<<port<<endl;
	newclient.sin_family =AF_INET;
	newclient.sin_port  =htons(port);
	newclient.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	
	int enable = 1;
	int sfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
    	perror("setsockopt(SO_REUSEADDR) failed");
    }
	int status = bind(sfd,(struct sockaddr*) &newclient,sizeof(newclient));
	
		perror("bind");

	
	int no_of_client=10;
	status =listen(sfd,no_of_client);
	if(status==-1)
	{
		perror("listen");
		exit(1);
	}
	
	int addrlen;

	
	int  usfd;
	struct sockaddr_un userv_addr,ucli_addr;
  	int userv_len,ucli_len;

	usfd = socket(AF_UNIX , SOCK_STREAM , 0);
	perror("Usocket");

  	bzero(&userv_addr,sizeof(userv_addr));

  	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	unlink(ADDRESS);
	userv_len = sizeof(userv_addr);

	if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
	perror("server: bind");

	listen(usfd, 5);

	ucli_len=sizeof(ucli_addr);


	int nusfd;
	cout<<"Before Accept\n";
	nusfd=accept(usfd, (struct sockaddr *)&ucli_addr, (socklen_t*)&ucli_len);
	

	for(; ; )
	{
		cout<<"inside While\n";
		int nsfd= accept(sfd,(struct sockaddr*) &newclient,(socklen_t *)&addrlen);
		send_fd(nusfd,nsfd);
		
	}
	
	
	
	
}
int main(int argc, char* argv[])
{
	struct sockaddr_in addrport,addrport1;
	struct sockaddr_in client_address;
	int addrlen=0;

	int sfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sfd==-1)
	{
		perror("socket");
		exit(1);
	}
	addrport.sin_family =AF_INET;
	addrport.sin_port  =htons(atoi(argv[1]));
	addrport.sin_addr.s_addr = inet_addr("127.0.0.1");
	int status = bind(sfd,(struct sockaddr*) &addrport,sizeof(addrport));
	if(status==-1)
	{
		perror("bind");
		exit(1);
	}
	/*int no_of_client=10;
	status =listen(sfd,no_of_client);
	if(status==-1)
	{
		perror("listen");
		exit(1);
	}
	int fd2;
	cout<<"Accepting\n";
	int nsfd = accept(sfd,(struct sockaddr*) &addrport,(socklen_t *)&addrlen);*/
	//cout<<"nsfd  "<<nsfd<<endl;
	pthread_t tid[10];
	int i=0;
	map<int,int> m;
	cout<<"Before Map\n";
	while(1)
	{
		char buffer[100];socklen_t length;
		cout<<"Before recvfrom\n";
		recvfrom(sfd,buffer,100,0,(sockaddr*)&addrport,&length);
		cout<<"buffer ="<<buffer<<endl;
		connectandpass(atoi(buffer));
		i++;
	}
	
	


	return 0;
}
