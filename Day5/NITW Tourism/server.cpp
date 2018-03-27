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
#define ADDRESS  "mysocket"
using namespace std;
map<int,int> agents;

int agentsPid[10];

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
 
 
void sendfdtoAgent(int nsfd)
{
	int  usfd;
	cout<<"Sending FD to Agent\n";
	struct sockaddr_un userv_addr,ucli_addr;
  	int userv_len,ucli_len;

	usfd = socket(AF_UNIX , SOCK_STREAM , 0);
	perror("socket");

  	bzero(&userv_addr,sizeof(userv_addr));

  	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	unlink(ADDRESS);
	userv_len = sizeof(userv_addr);
	int status;
	if((status = bind(usfd, (struct sockaddr *)&userv_addr, userv_len))==-1)
	perror("server: bind");
	cout<<"After Bind "<<status<<endl;
	listen(usfd, 5);

	ucli_len=sizeof(ucli_addr);

	int nusfd;
	cout<<"Accepting for Agent\n";
	nusfd=accept(usfd, (struct sockaddr *)&ucli_addr, (socklen_t*)&ucli_len);
	cout<<"Accepted  "<<nusfd<<endl;
	cout<<"Now Calling Send_fd\n";
	send_fd(nusfd,nsfd);

}
void createAgents()
{
	cout<<"Creating Agents\n";
	int noOfAgents=2;
	agentsPid[1] =fork();
	
	if(agentsPid[1]==0)
	{
		execvp("./A1",NULL);
	}
	else
	{
		cout<<"AgentPid = "<<agentsPid[1]<<endl;
	}
	agentsPid[2] =fork();
	if(agentsPid[2]==0)
	{
		execvp("./A2",NULL);
	}
	else
	{
		cout<<"AgentPid = "<<agentsPid[2]<<endl;
	}
	cout<<"Two Agents Created\n";
}
void initialiseAgents()
{
	cout<<"Initialising Services to Agents\n";
	agents[1]=1;
	agents[3]=1;
	agents[7]=1;
	agents[2]=2;
	agents[2]=2;
	cout<<"Initialised Services to Agents\n";
}
int main(int argc ,char* argv[])
{
	if(argc!=2)
	{
		cout<<"Provide Port too\n";
		exit(0);
	}
	
	int sfd;
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t cli_len;
	int port_no=atoi(argv[1]);

	if((sfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	perror("\n socket ");
	else printf("\n socket created successfully Server");

	bzero(&serv_addr,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_no);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(bind(sfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))==-1)
	perror("\n bind : ");
	else printf("\n bind successful ");

	listen(sfd,10);
	cout<<"Listening \n";
	int nsfd;
	
	for(; ; )
	{
		cout<<"Accepting\n";
		if((nsfd = accept(sfd , (struct sockaddr *)&cli_addr , &cli_len))==-1)
		perror("\n accept ");
		char buffer[50];
		cout<<"Accepted = "<<nsfd<<endl;
		recv(nsfd,buffer,50,0);
		int t=atoi(buffer);
		cout<<"T= "<<t<<endl;
		recv(nsfd,buffer,50,0);
		int l =atoi(buffer);
		cout<<"T and L from Clients are"<<t<<"  "<<l<<endl;
		initialiseAgents();
		createAgents();
		if(0==kill(agentsPid[agents[t]],0))
		{
			cout<<"Process is still running\n";
		}
		cout<<"pids ="<<agentsPid[agents[t]]<<endl;
		cout<<"Kill ="<<kill(agentsPid[agents[t]],SIGUSR1);
		if(0==kill(agentsPid[agents[t]],0))
		{
			cout<<"Process is still running After Kill\n";
		}
		cout<<" Kill System Call over\n";
		sendfdtoAgent(nsfd);
		
	}
}



