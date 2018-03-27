#include<stdio.h> //for printf
#include<string.h> //memset
#include<sys/socket.h>    //for socket ofcourse
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include<sys/time.h>
#include<unistd.h>
#define PACKETSIZE 64

	struct icmphdr hdr;
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
void display(struct icmphdr pckt,char* ipaddr,float timetaken)
{
	int sizeofpacket =sizeof (pckt);
	dprintf(1,"%d bytes from %s: icmp_seq = %d time = %f ms\n",sizeofpacket,ipaddr,pckt.un.echo.sequence,timetaken);
}
int main(int argc,char* argv[])
{
	int rsfd =socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
	int val=255;

	
	struct sockaddr_in addr,claddr;
	addr.sin_family =AF_INET;
	addr.sin_addr.s_addr =inet_addr(argv[1]);
	if ( setsockopt(rsfd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
		perror("Set TTL option");
		int cnt=0;
		int one=1;const int *val1 = &one;
	 
		while(1)
		{
			cnt++;
			if(cnt==64)
				break;
			struct icmphdr pckt,rpckt;
			socklen_t len=sizeof (addr);
			pckt.type = ICMP_ECHO;
			pckt.un.echo.id = getpid();
			pckt.un.echo.sequence=cnt;
			//bzero(pckt.msg,PACKETSIZE-sizeof(struct icmphdr));
			//strcpy(pckt.msg,"Hello");
			pckt.checksum=checksum(&pckt,sizeof(pckt));
			struct timeval stop, start;
			gettimeofday(&start, NULL);
			sendto(rsfd,&pckt,sizeof(struct icmphdr),0,(struct sockaddr*)&addr,sizeof(addr));	
			recvfrom(rsfd, &rpckt, sizeof(struct icmphdr), 0, (struct sockaddr*)&claddr, &len);
			gettimeofday(&stop, NULL);
			display(rpckt,inet_ntoa(claddr.sin_addr),(stop.tv_usec - start.tv_usec)/1000.0);
			
		}
		
		
	
	
}
