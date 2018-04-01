#include <ace/Reactor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/Synch.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Log_Msg.h>
#include <ace/streams.h>
#include <ace/INET_Addr.h>
#include <bits/stdc++.h>
#include <errno.h>
using namespace std;

int main()
{
	ACE_INET_Addr server("127.0.0.1:8888"),client;
	ACE_SOCK_Dgram server_data(server);
 	char message[100];
	size_t sent_data_length = server_data.recv(message,50,client);
	printf("Client details: Host Name: %s Port Number: %d\n",client.get_host_name(), client.get_port_number());
	
	cout<<"Send  Data length ="<<sent_data_length<<endl;
	if (sent_data_length == -1) 
	{
	  printf("Error in data transmission\n");
	}
  	else
  	{
  		cout<<"Message = "<<message<<endl;
  	}
 
	server_data.close();
}
