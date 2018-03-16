#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
using namespace std;
int main()
{
	char msg[1000];
	ofstream fout("output.txt");
	bzero(msg,1000);
	while(read(0,msg,1000) > 0)
	{
		cout<<strlen(msg)<<endl;
		cout<<msg;
	}
	return 0;
}

