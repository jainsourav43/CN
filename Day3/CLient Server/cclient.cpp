#include<bits/stdc++.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>

using namespace std;

string sub_str(char* cstr,int ind,int len)
{
	string str = string(cstr);
	string sub = str.substr(ind,len);
	return sub;
}

void* readfifo(void* args)
{
	int cfd = atoi((char*)args);
	cout<<"in read"<<endl;
	while(1)
	{
		char s[50];
		read(cfd,s,50);
		printf("%s\n",s);
	}
}

void* writefifo(void* args)
{
	int sfd = atoi((char*)args);
	cout<<"in write"<<endl;
	while(1)
	{
		char s[50];
		printf("\nEnter a message: ");
		fgets(s,50,stdin);
		write(sfd,s,strlen(s));
	}
}

int main(int argc,char* argv[])
{
	char* sfifo = "/tmp/server";
	char* cpath = argv[1];
	char* cfifo = (char*)sub_str(cpath,1,strlen(cpath)-1).c_str();
	mkfifo(sfifo,0666);
	mkfifo(cfifo,0666);
	cout<<cfifo<<endl;
	int cfd = open(cfifo,O_RDONLY);
	cout<<"here"<<endl;
	int sfd = open(sfifo,O_WRONLY);
	
	cout<<"cfd: "<<cfd<<endl;
	cout<<"sfd: "<<sfd<<endl;
	
	//request server for connection
	write(sfd,cpath,strlen(cpath));
	
	pthread_t rt,wt;
	
	cout<<"yo"<<endl;
	
	pthread_create(&rt,NULL,readfifo,(void*)cfd);
	pthread_create(&wt,NULL,writefifo,(void*)sfd);
	
	pthread_join(rt,NULL);
	pthread_join(wt,NULL);
	
	close(cfd);
	close(sfd);
	
	return 0;
}
