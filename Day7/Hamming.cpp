#include<iostream>
#include<bits/stdc++.h>
#include<sstream>
using namespace std;
int main()
{
	string input;
	cout<<"Enter the Data:  ";
	cin>>input;
	int databits = input.length();
	int tempparitybits = log2(databits);
	int paritybits = log2(databits+tempparitybits)+1;
	if(pow(2,paritybits)<(databits+paritybits+1))
	{
		paritybits++;
	}
	string	ans =string(databits+paritybits+1,'0');
	int i=0,j=0;
	for(i=1;i<ans.length();i++)
	{
		if((log2(i)-(int)log2(i))==0)
		{
			continue;
		}
		else
		{
			ans[i]= input[j++];
		}
	}
	int check=0; 
	int total=databits+paritybits;
	for(i=1;i<ans.length();i++)
	{
		int count=0;
		if((log2(i)-(int)log2(i))==0)
		{

			check= pow(2,i-1);
			for(j=check+1;j<=total;j++)
			{
				if(check&j)
				{
					count++;
				}
			}
			if(count%2)
			{
				ans[(int) log2(i)] = '1';
			}
			else
			{
				ans[(int) log2(i)] = '0';
			}
			
		}
	}
	cout<<"Ans :   "<<ans.substr(1,ans.length()-1)<<endl;
}
