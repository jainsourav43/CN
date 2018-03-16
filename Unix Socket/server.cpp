#include<bits/stdc++.h>
#include<sys/un.h>
#include<unistd.h>
#include <sys/socket.h>
#include<sys/select.h>
#include<sys/types.h>
#include<fcntl.h>


#define path "./socket"



using namespace std;

int main()
{
	sockaddr_un clientaddr,servaddr;
	int sockfd = socket(AF_LOCAL,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sun_family =AF_LOCAL;
	strcpy(servaddr.sun_path,path);
	bind(sockfd,(sockaddr*)&servaddr,sizeof(servaddr));
	listen(sockfd,1);
	
	int new_socket = open("file.txt",O_RDONLY);
	for(; ;)
	{
		socklen_t len =sizeof(clientaddr);
		int connfd=accept(sockfd,(sockaddr*)&clientaddr,&len);
		if(connfd<0)
		{
			cout<<"Accept Error\n";
			exit(0);
		}
		
		iovec iov[1]={0};
		const char *str ="file";
		iov[0].iov_base =(void*)str;
		iov[0].iov_len=strlen(str)+1;
		msghdr parentmsg;
		memset(&parentmsg,0,sizeof(parentmsg));
		cmsghdr *cmsg;
		
		
		char cmsgbuff[CMSG_SPACE(sizeof(sockfd))]={};
		parentmsg.msg_name=NULL;
		parentmsg.msg_namelen=0;
		parentmsg.msg_iov =iov;
		parentmsg.msg_iovlen =1;
		parentmsg.msg_control =cmsgbuff;
		parentmsg.msg_controllen=sizeof(cmsgbuff);
		
		
		
		cmsg=CMSG_FIRSTHDR(&parentmsg);
		
		 if(cmsg == NULL)
		 {
		   cout<<"\ncmsg is null in server side.\n";
		 }
		 
		 cmsg->cmsg_level =SOL_SOCKET;
		 cmsg->cmsg_type= SCM_RIGHTS;
		 cmsg->cmsg_len =CMSG_LEN(sizeof(sockfd));
		 memcpy(CMSG_DATA(cmsg),&sockfd,sizeof(sockfd));
		 parentmsg.msg_controllen =cmsg->cmsg_len;
		 
		 if(sendmsg(connfd,&parentmsg,0)<0)
		 {
		 	cout<<"Send Error";
		 	exit(0);
		 	
		 }
		return 0;

	}
			
}
