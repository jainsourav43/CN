#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;
string  substr(char* s,int start,int length)
{
	string str =  string(s);
	string str1 = str.substr(start,length);
	return str1;
}
int main(int argc, char const *argv[])
{
	int c;
	char *newfifo = "/tmp/mainserver";
	cout<<mkfifo(newfifo,0666)<<endl;
	char s1[50],s2[50];
	map<int,map<int,int>>m;
	int count=0;
	while(1)
	{
		int fd = open(newfifo,O_RDONLY);
		cout<<"Before Read\n";
		read(fd,s1,50);
		if(s1[0] == '1')
		{
			cout<<"Creating Pipes\n";
			char *s2 = (char*)substr(s1,1,8).c_str();
			int id= s1[7]-'0';
			mkfifo(s2,0666);
			int severid = s1[13]-'0';
			if(m.find(severid)==m.end())
			{
				cout<<""
				map<int,int> temp;
				char *s3 = (char*)substr(s1,12,6).c_str();
				string s4 = "/tmp/"+string(s3)
				char *s5=(char*)s4.c_str();
				int fd = open(s2,O_WRONLY);
				temp[id] =fd;
				m[severid] = temp;
			}
			else
			{
				int fd = open(s2,O_WRONLY);
				m[severid][id] =fd;
			}
			
			// count++;
			cout<<"Pipe Created\n";
		}
		else if(s1[0] == '2')
		{
			int id =s1[2]-'0';
			map<int,int> :: iterator it = m.begin();
			char *message = (char*)substr(s1,1,strlen(s1)-1).c_str(); 
			while(it!=m.end())
			{
				if(it->first !=id)
				{
					write(it->second,message,strlen(message));

				}
				it++;
			}

		}
		else if(s1[0]=='3')
		{
			int id = s1[2]-'0';
			m.erase(id);
			close(m[id]);

		}
		close(fd);


	}
	return 0;
}