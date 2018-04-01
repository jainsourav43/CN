#include <ace/Reactor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Synch.h>
#include <ace/Log_Msg.h>
#include <ace/streams.h>
#include <ace/INET_Addr.h>
#include <bits/stdc++.h>
#include <errno.h>
using namespace std;

int main()
{
	ACE_INET_Addr server("127.0.0.1:8888"),client;
	ACE_SOCK_Dgram client_data(client);
	
	char message[100];
	 strcpy(message,"Hello World!\n");
	 cout<<"Trying to Send \n"<<message<<endl;
	size_t sent_data_length = client_data.send(message,strlen(message) + 1, server);
	if (sent_data_length == -1) 
	  printf("Error in data transmission\n");
	else
	printf("Data Sent\n");
	 
	client_data.close();
}
