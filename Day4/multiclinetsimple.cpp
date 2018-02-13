#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<pthread.h>

using namespace std;
string  substr(char* s,int start,int length)
{
	string str =  string(s);
	//cout<<"s = "<<s<<endl;
	//cout<<"Starr = "<<start<<endl;
	string str1 = str.substr(start,length);
	return str1;
}
void *runnerread(void * p)
{
	cout<<"Inside read\n";
	int rfd= *(int*) p;
	cout<<"Rfd = "<<rfd<<endl;
	while(1)
	{
		char s[50];
		cout<<"Inside Read s = "<<s<<endl;
		read(rfd,s,50);
		printf("%s\n",s );
	}

}
void *runnerwrite(void* p)
{
	int wfd= *(int*) p;
	cout<<"Inside Write\n";
	while(1)
	{
		printf("Enter the Message  Starting with 2 followed by name and Message\n");
		char s[50];
		fgets(s,50,stdin);
		write(wfd,s,strlen(s));

	}

}
int main(int argc, char  *argv[])
{
	cout<<argv[1]<<endl;
	int c;
	cout<<"C\n";
	char *newfifo = "/tmp/server";
	mkfifo(newfifo,0666);
	cout<<"Fifo\n";
	int wfd = open(newfifo,O_WRONLY);
	write(wfd,argv[1],strlen(argv[1]));
	cout<<"Written Successfully\n";

	char *myfifo = (char*)substr(argv[1],2,strlen(argv[1])-2).c_str();
	cout<<"myfifo 
	= "<<myfifo<<endl;
	//cout<<myfifo<<endl;
	mkfifo(myfifo,0666);
	cout<<"myfifo"<<endl;
	int rfd = open(myfifo,O_RDONLY);
	cout<<"rfd,wfd = "<<rfd<<" "<<wfd<<endl;
	cout<<"Before Read\n";


	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,runnerread,(void*)&rfd);
	pthread_create(&tid2,NULL,runnerwrite,(void*)&wfd);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	return 0;
}
