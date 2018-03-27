#include<stdio.h> //for printf
#include<string.h> //memset
#include<sys/socket.h>    //for socket ofcourse
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include<unistd.h>
#define PACKETSIZE 64

	//char msg[PACKETSIZE-sizeof(struct icmphdr)];


unsigned short checksum(void *b, int len)
{	unsigned short *buf = b;
	unsigned int sum=0;
	unsigned short result;

	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++;
	if ( len == 1 )
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

void display(char* ms)
{
	dprintf(1,"Message: %s \n",ms);
}
int main()
{
	int rsfd =socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
	int val=255;

	
	int cnt=0;
	int check=10;
	while(check--)
	{
		cnt++;
		struct icmphdr pckt;
		struct sockaddr_in addr;
		socklen_t len=sizeof (addr);
		recvfrom(rsfd, &pckt, sizeof(struct icmphdr), 0, (struct sockaddr*)&addr, &len);
		dprintf(1,"ID= %d\n ",pckt.un.echo.id);
		dprintf(1," Before one Ip = %s\n",inet_ntoa(addr.sin_addr));
		struct in_addr temp ;
		temp.s_addr =addr.sin_addr.s_addr;
		dprintf(1,"Ip = %d\n",pckt.un.echo.sequence);
		struct icmphdr spckt;

		bzero(&spckt,64);
		spckt.type = ICMP_ECHO;
		spckt.un.echo.id = getpid();
		spckt.un.echo.sequence=cnt;
		dprintf(1,"Cnt: %d \n",cnt);
		spckt.checksum=checksum(&pckt,sizeof(pckt));
		addr.sin_addr.s_addr = inet_addr("192.168.43.224");
		dprintf(1,"Ip = %s\n",inet_ntoa(addr.sin_addr));
		sendto(rsfd,&spckt,sizeof(struct icmphdr),0,(struct sockaddr*)&addr,sizeof(addr));	
		
	}
}
