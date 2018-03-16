#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

void itoa(char* str,int n)
{
	int i = 0;
	if(n==0)
	{
		strcpy(str,"0");
		return;
	}
	
	while(n)
	{
		int rem = n%10;
		str[i] = (rem+'0');
		n /= 10;
		i++;
	}	
	str[i] = '\0';
	reverse(str,str+i);
}

int main(int argc,char* argv[])
{
	int n1 = atoi(argv[1]);
	int n2 = atoi(argv[2]);
	
	char buf[100];
	bzero(buf,100);
	strcpy(buf,"ADDER: ");
	strcat(buf,argv[1]);
	strcat(buf,"+");
	strcat(buf,argv[2]);
	strcat(buf,"=");
	char ans[20];
	itoa(ans,n1+n2);
	strcat(buf,ans);
	cout<<buf<<endl;
	return 0;
}
