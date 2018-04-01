#include <ace/Reactor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/Synch.h>
#include <ace/Log_Msg.h>
#include <ace/streams.h>
#include <ace/INET_Addr.h>
#include <bits/stdc++.h>
#include <errno.h>
using namespace std;
int main()
{

	
	ACE_INET_Addr server("127.0.0.1:8887");
	ACE_SOCK_Stream client_stream;
	ACE_SOCK_Connector client_connector;
	if(client_connector.connect(client_stream,server)==-1)
	{
		printf("Failure to Establish Connection\n");
		return 0;
	}
	
	int sent_data =client_stream.send_n("Hello World",12,0);
	cout<<"Sent Data ="<<sent_data<<endl;
	client_stream.close();
}
