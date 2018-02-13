#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

using namespace std;

string sub_str(char* cstr,int ind,int len)
{
	string str = string(cstr);
	string sub = str.substr(ind,len);
	return sub;
}

int main()
{
	int c;
	char *newfifo = "/tmp/server";
	cout<<mkfifo(newfifo,0666)<<endl;
	char s1[50],s2[50];
	map<int,int>m;
	int count=0;
	while(1)
	{
		cout<<"Hi\n";
		int fd = open(newfifo,O_RDONLY);
		cout<<"Fuck\n";
		char sinput[50];
		cout<<fd<<endl;
		read(fd,sinput,50);
		switch(sinput[0])
		{
			case '1':
			{	
				//client connect request
				char* cpath = (char*)sub_str(sinput,1,strlen(sinput)-1).c_str();
				int cid = cpath[7]-'0';
				int cfd = open(cpath,O_WRONLY);
				m[cid] = cfd;
			}
			case '2':
			{
				//client message request
				char* cmsg = (char*)sub_str(sinput,1,strlen(sinput)-1).c_str();
				int cid = cmsg[2]-'0';
				map<int,int>::iterator it = m.begin();
				for(;it!=m.end();it++)
				{
					if(it->first!=cid)
					{
						int cfd = it->second;
						write(cfd,cmsg,strlen(cmsg));
					}
				}
			}
			case '3':
			{
				//client disconnect request
				char* cpath = (char*)sub_str(sinput,1,strlen(sinput)-1).c_str();
				int cid = cpath[2]-'0';
				close(m[cid]);
				m.erase(cid);
			}
		}
		close(fd);
	}
	
	return 0;
}
