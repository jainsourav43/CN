#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/un.h>
#define path "./socket"
#define MAXSIZE 10
#define SEM "/sem"
using namespace std;

int i;
bool first =false;
key_t key;
int sock;
char *shm;
sem_t *sem;
int *pids;
int myindex;
void handler1(int sig)
{
	cout<<"Inside Handler 1\n";
   cout<<"Handling Signal\n";

   
   char buff[100];
   recv(sock,buff,10,0);
   sleep(2);
   cout<<buff<<endl;
   cout<<"Myindex,pids[myindex] = "<<myindex<<"   "<<pids[myindex]<<endl; 
      sem_wait(sem);
   if(myindex+1<MAXSIZE && pids[myindex+1]!=0)
   {
      kill(pids[myindex+1],SIGUSR1);
   }
   else
   { 
      kill(pids[0],SIGUSR1);
   }
   
   sem_post(sem);
   
   
   
}



void handler2(int sig)
{
	cout<<"Inside Handler 2\n";
      int sockfd;
	   sockaddr_un cliaddr,servaddr;
	   sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	   cout<<"Sockfd = "<<sockfd<<endl;
	 	//system("rm socket");
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
		
		 cout<<"check = "<<check<<endl;
		 cout<<"\nmessage sent\n";
		 
		 close(sockfd);
		 close(connfd);
	
	
	
}

void getPassfd()
{
	if(!first){
	cout<<"Not First\n";
      sem_wait(sem);
      
      if(myindex-1>=0&& pids[myindex-1]!=0)
      {
      	cout<<"pid[index-1] ="<<pids[myindex-1]<<endl;
         kill(pids[myindex-1],SIGUSR2);
      }
      else
      { 
      	      	cout<<"pid[index-1] ="<<pids[0]<<endl;
         kill(pids[0],SIGUSR2);
      }
      
      sem_post(sem);
      }
      
    	sleep(5);
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
	 cout<<"Sock  = "<<sock<<endl;
     

	  
	  
	  
   
}

int writepid()
{
	
  		cout<<"Pid Before\n";
  	for(int i=0;i<MAXSIZE;i++)
    {
      	cout<<pids[i]<<" ";
    }
   sem_wait(sem);
   for(int i=0;i<MAXSIZE;i++)
   {
      if(pids[i]==0)
      {
         pids[i]=getpid();
          cout<<"Pid After\n";
	   	for(int i1=0;i1<MAXSIZE;i1++)
		  {
		  	cout<<pids[i1]<<" ";
		  }
         sem_post(sem);
         return i;
      }
   }
   
   sem_post(sem);
   cout<<"Pid After\n";
   	for(int i=0;i<MAXSIZE;i++)
      {
      	cout<<pids[i]<<" ";
      }
   return 0;
}




int main()
{  
   //system("rm socket");
   signal(SIGUSR1,handler1);
   signal(SIGUSR2,handler2);
   cout<<"Enter the Key\n";
   cin>>key;
   
   int shmid =shmget(key,MAXSIZE*sizeof(int),0644);
   cout<<"shmid = "<<shmid<<endl;
   char *sem_name;
   sem_name =new char[10];
   cout<<"Enter Semaphore Name\n";
   cin>>sem_name;
   if(shmid<0)
   { 
      cout<<"Creating First Shm\n";
      first=true;
      shmid =shmget(key,MAXSIZE*sizeof(int),IPC_CREAT|0644);
      
         cout<<"shmid = "<<shmid<<endl;
	  cout<<"check"<<endl;
      sem= sem_open(sem_name,O_CREAT,0644,1);
      cout<<"After Opening Semaphore\n";
   }
   else
   {
      cout<<"Opening Sem\n";
      sem=sem_open(sem_name,0);
   }
   		cout<<"Before PIDS";
      pids =(int*)shmat(shmid,NULL,0);
     if(first)
     {
		  for(int i1=0;i1<MAXSIZE;i1++)
		  {
		  	pids[i1]=0;
		  }
     }
  
        myindex= writepid();
         cout<<"Pid After\n";
   	  for(int i1=0;i1<MAXSIZE;i1++)
      {
      	cout<<pids[i1]<<" ";
      }
	cout<<"MYIndex = "<<myindex<<endl;
   if(first)
   {
      cout<<"inside First\n";
     
      socklen_t len;
      sock= socket(AF_INET,SOCK_STREAM,0);
      if(sock<0)
      {
         cout<<"Error";
         exit(0);
      }
      else
      {
      	cout<<"Socket Created\n";
      }
      sockaddr_in addr;
      addr.sin_family =AF_INET;
      addr.sin_port =htons(8080);
      addr.sin_addr.s_addr =inet_addr("127.0.0.1");
      if(connect(sock,(sockaddr*) &addr,sizeof(addr))<0)
      {
         cout<<"Connect Error";
         exit(1);
      }
      else
      {
      	cout<<"FIrst Client Got Connected\n";
      }
      if(first)
      {
      	kill(getpid(),SIGUSR1);
      }
      while(1)
      {
      cout<<"I am in while\n";
      }
      
      
   }
   else
   {
      getPassfd();  
      while(1)
      {
      }
        
   }
   

 
  
  
  
  
  
  
  
   
   
   
   
   
   
   
   return 0;
}
