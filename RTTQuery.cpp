#include "RTTQuery.h"

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/signal.h>
#include <string.h>
#include <iostream>
#include <time.h>

#define DEFDATALEN      56
#define MAXIPLEN        60
#define MAXICMPLEN      76

using namespace std;

list<double> RTTQuery::getRTTList(list<string> ipList)
{
	list<double> RTTList;
	int ipListSize = ipList.size();
	for(int i=0;i<ipListSize;i++)
	{
		RTTList.push_front(getRTT(ipList.front()));
		ipList.pop_front();
	}
	return RTTList;
}

int RTTQuery::checksum(unsigned short *buf,int sz)
{
	int nleft = sz;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;
	return (ans);
}

double RTTQuery::diff_in_ms(struct timespec t1, struct timespec t2)
{
	struct timespec diff;
	if (t2.tv_nsec-t1.tv_nsec < 0) {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
	} else {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	}
	return (diff.tv_sec + diff.tv_nsec / 1000000000.0)*1000;
}

double RTTQuery::getRTT(string ip)
{
	struct hostent *h;
	struct sockaddr_in pingaddr;
	struct icmp *pkt;
	int pingSocket;
	int c;
	char packet[DEFDATALEN + MAXIPLEN + MAXICMPLEN];
	struct timespec start,end;
	

	pingSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	memset(&pingaddr,0,sizeof(struct sockaddr_in));
	pingaddr.sin_family=AF_INET;
	h=gethostbyname(ip.c_str());
	memcpy(&pingaddr.sin_addr,h->h_addr,sizeof(pingaddr.sin_addr));
	pkt=(struct icmp *)packet;
	memset(pkt,0,sizeof(packet));
	pkt->icmp_type=ICMP_ECHO;
	pkt->icmp_cksum=checksum((unsigned short *) pkt, sizeof(packet));;

	c=sendto(pingSocket,packet,sizeof(packet),0,(struct sockaddr *)&pingaddr,sizeof(struct sockaddr_in));
	
	clock_gettime(CLOCK_REALTIME,&start);
	while(1)
	{
		struct sockaddr_in pktReply;
		unsigned int pktLenth = sizeof(pktReply);

		c=recvfrom(pingSocket,packet,sizeof(packet),0,(struct sockaddr *)&pktReply,&pktLenth);
		if(c>=76)
		{
			struct iphdr *iphdr=(struct iphdr *)packet;
			pkt=(struct icmp *)(packet + (iphdr->ihl << 2));	//skip ip header
			if(pkt->icmp_type == ICMP_ECHOREPLY)
			{
				cout << "get reply" << endl;
				clock_gettime(CLOCK_REALTIME,&end);
				return diff_in_ms(start,end);
			}
		}
	}
}
