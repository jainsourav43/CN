#include<stdio.h> //for printf
#include<string.h> //memset
#include<sys/socket.h>    //for socket ofcourse
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include <arpa/inet.h>
void printip()
{
	
}
int log_txt=1;
struct sockaddr_in source,dest;
void ip_header(unsigned char* buffer,int buflen)
{
	struct iphdr *ip = (struct iphdr*)(buffer);

	int iphdrlen =ip->ihl*4;
	
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = ip->saddr;  
	if(strcmp(inet_ntoa(source.sin_addr),"1.2.3.4")==0)   
	return;
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = ip->daddr;     

	dprintf(log_txt , "\nIP Header\n");

	dprintf(log_txt , "\t|-Version              : %d\n",(unsigned int)ip->version);
	dprintf(log_txt , "\t|-Internet Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
	dprintf(log_txt , "\t|-Type Of Service   : %d\n",(unsigned int)ip->tos);
	dprintf(log_txt , "\t|-Total Length      : %d  Bytes\n",ntohs(ip->tot_len));
	dprintf(log_txt , "\t|-Identification    : %d\n",ntohs(ip->id));
	dprintf(log_txt , "\t|-Time To Live	    : %d\n",(unsigned int)ip->ttl);
	dprintf(log_txt , "\t|-Protocol 	    : %d\n",(unsigned int)ip->protocol);
	dprintf(log_txt , "\t|-Header Checksum   : %d\n",ntohs(ip->check));
	dprintf(log_txt , "\t|-Source IP         : %s\n", inet_ntoa(source.sin_addr));
	dprintf(log_txt , "\t|-Destination IP    : %s\n",inet_ntoa(dest.sin_addr));
	//dprintf(log_txt,"Size =%d\n",buflen);
	char* data=buffer+sizeof(struct iphdr)+sizeof (struct tcphdr);
	dprintf(log_txt,"\nData\n");
	int i;
	int remaining_data = buflen - ( sizeof(struct iphdr) + sizeof(struct tcphdr));
//	dprintf(log_txt,"Remaining %d \n",remaining_data);
	for(i=0;i<remaining_data;i++)
	{
		if(i!=0 && i%16==0)
			dprintf(log_txt,"\n");
		dprintf(log_txt," %.2X ",data[i]);
	}

	dprintf(log_txt,"\n");

//	dprintf(log_txt,"\t|-Data   :%x ",data);
	
}

int main()
{
	int rsfd =socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
	
	
	if(rsfd<0)
	{
		perror("socket");
		exit(0);
	}
	
	    struct iphdr *iph = (struct iphdr *) malloc(65535);
	
	    struct sockaddr_in sin;
	    socklen_t socklength;
	    while(1)
	    {

	    	bzero(iph,65535);
	    	int buflen= recvfrom(rsfd,iph,65535,0,(struct sockaddr*)&sin,&socklength);
	    	//dprintf(log_txt,"Below Len= %d\n",buflen);
			ip_header((char*)iph,buflen);
		}
    	
    	
	    
	    
	    
}
