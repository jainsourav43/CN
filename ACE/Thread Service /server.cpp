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
#include<ace/Thread.h>
using namespace std;

ACE_SOCK_Stream client_stream;

void* service(void* arg)
{
	int k = *(int*)arg;
	char buffer[128];
	cout<<"Client Number  = "<<k<<endl;
	if (client_stream.recv_n(buffer, 40, 0)==-1) 
	{ 
	   printf("Error in reading client data!\n");   
	   return 0;
	} 
	else 
	{ 
	   printf("Client message: %s\n", buffer);
	}
	client_stream.close();

	
}
int main()
{

		ACE_INET_Addr server("127.0.0.1:8887"),client;
		ACE_SOCK_Acceptor client_responder(server);
		ACE_Time_Value timeout(50);
		int n=0;
		int a[10];
		while(1)
		{
			if (-1 == client_responder.accept(client_stream, &client, &timeout)) 
			{ 
				printf("Connection not established with client!\n");
				return 0;
			} 
			else 
			{ 
			 a[n]=n;
	   		 printf("Client details: Host Name: %s Port Number: %d\n",client.get_host_name(), client.get_port_number());
	   		 ACE_Thread::spawn((ACE_THR_FUNC)service,&a[n]);
	   		 n++;
			}
		
			
		}

		
}


