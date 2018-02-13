#include<iostream>
#include<bits/stdc++.h>
using namespace std;
string subtract(string s1,string s2)
{
	int i; string ans="";
	for(i=0;i<s1.length();i++)
	{
		if(s1[i]==s2[i])
		{
			ans=ans+'0';
		}
		else
		{
			ans=ans+'1';
		}
	}
	return ans.substr(1,ans.length()-1);
}
int main()
{
	string input,poly,polystr="";
	int degree;
	cout<<"Enter the Input\n";
	cin>>input;
	string ans=input;
	cout<<"Enter the Polynomial\n";
	cin>>poly;
	string divsor;
	int i=0,temp=0,j;
	while(i<poly.length()&&(poly[i]>='A'))
	{
		i++;
	}
	while(i<poly.length()&&poly[i]>='0'&&poly[i]<='9')
	{
		temp = temp*10+(poly[i]-'0');
		i++;
	}
	degree =temp;
	cout<<"Degree = "<<degree<<endl;
	for(i=0;i<temp;i++)
	{
		input = input+'0';
	}
	int dupdeg=degree,check=0;
	for(i=0;i<poly.length();i++)
	{
		check=0;
		while(i<poly.length()&&(poly[i]>='A'))
		{
			check=1;
			i++;
		}
		temp=0;
		while(i<poly.length()&&poly[i]>='0'&&poly[i]<='9')
		{
			temp = temp*10+poly[i]-'0';
			i++;
		}
		if(check==0)
		{
			temp=0;
			for(j=dupdeg;j>temp;j--)
			{
				polystr = polystr+'0';
				dupdeg--;
			}
			polystr = polystr+"1";
		}
		else
		{
		
			for(j=dupdeg;j>temp;j--)
			{
				polystr = polystr+'0';
				dupdeg--;
			}
		
			polystr = polystr+"1";
		}	
			
				dupdeg--;
			
	}
	if(dupdeg==0)
	{
		polystr=polystr+'0';
	}
	
	cout<<"Polynomial in Numbers  = "<<polystr<<endl;
	//cout<<"input ="<<input<<endl;
	string current  =input.substr(0,polystr.length());
	string temppoly = string(polystr.length(),'0');
	string tempstr="1";
	for(i=polystr.length();i<=input.length();i++)
	{	
	//cout<<"cureent = "<<current<<endl;
		if(tempstr[0]== '1')
		{
			tempstr=subtract(current,polystr);
			if(i<input.length())
			current =tempstr+input[i];
		}
		else
		{
			tempstr=subtract(current,temppoly);
			if(i<input.length())
			current =tempstr+input[i];	
		}
		//cout<<tempstr<<endl;
		//cout<<i<<endl;
	}
//	cout<<tempstr<<endl;
	ans =ans+tempstr;
	cout<<"ans   = "<<ans<<endl;
		
	
}
