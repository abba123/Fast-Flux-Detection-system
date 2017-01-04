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
#include <list>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include "Traceroute.h"

#define DEFDATALEN      56
#define MAXIPLEN        60
#define MAXICMPLEN      76

using namespace std;

int flag=0;

int Traceroute::checksum(unsigned short *buf,int sz)
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

string Traceroute::sendTraceroutePkt(string ip, unsigned short ttl)
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
	fcntl(pingSocket,F_SETFL,O_NONBLOCK);
	h=gethostbyname(ip.c_str());
	memcpy(&pingaddr.sin_addr,h->h_addr,sizeof(pingaddr.sin_addr));
	pkt=(struct icmp *)packet;
	memset(pkt,0,sizeof(packet));
	pkt->icmp_type=ICMP_ECHO;
	pkt->icmp_cksum=checksum((unsigned short *) pkt, sizeof(packet));;

	int setTTL=setsockopt(pingSocket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
	
	c=sendto(pingSocket,packet,sizeof(packet),0,(struct sockaddr *)&pingaddr,sizeof(struct sockaddr_in));
	
		struct sockaddr_in pktReply;
		unsigned int pktLenth = sizeof(pktReply);
		usleep(500000);
		c=recvfrom(pingSocket,packet,sizeof(packet),0,(struct sockaddr *)&pktReply,&pktLenth);	
		if(c>0)
		{
			struct iphdr *iphdr=(struct iphdr *)packet;
			pkt=(struct icmp *)(packet + (iphdr->ihl << 2));	//skip ip header
			struct iphdr* ip_reply = (struct iphdr *) packet;
			char srcIP[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(ip_reply->saddr), srcIP, INET_ADDRSTRLEN);
			if(pkt->icmp_type == 0)
			{
				flag=1;
			}
			return srcIP;
		}
		else
		{
			return "0.0.0.0";
		}
}

string Traceroute::getDomain(string ip)
{
	struct in_addr ipv4;
	struct hostent *record;
	string dns;
	inet_pton(AF_INET, ip.c_str(), &ipv4);
	record=gethostbyaddr(&ipv4,4,AF_INET);
	if (record==NULL) return "";
	dns=record->h_name;
	dns=dns.substr(dns.rfind(".",dns.rfind(".")-1)+1);
	return dns;
}

list<string> Traceroute::getTraceroute(string ip)
{
	flag=0;
	list<string> tracerouteList;
	unsigned short ttl=1;
	while(flag!=1)
	{
		string srcIP=sendTraceroutePkt(ip,ttl);
		string domain=getDomain(srcIP);
		if(domain!="")	tracerouteList.push_back(domain);
		ttl++;
	}
	tracerouteList.unique();
	return tracerouteList;
}

list<string> Traceroute::getTracerouteList(list<string> ipList)
{
	list<string> tracerouteList;
	int size=ipList.size();
	for(int i=0;i<size;i++)
	{
		list<string> tmp=getTraceroute(ipList.front());
		tracerouteList.splice(tracerouteList.end(),tmp);
		ipList.pop_front();
	}
	return tracerouteList;
}

int getListCount(list<string> l,string str)
{
	int size=l.size();
	int count=0;
	for(int i=0;i<size;i++)
	{
		if(str.compare(l.front())==0)	count++;
		l.pop_front();
	}
	return count;
}

double Traceroute::TracerouteEntropy(list<string> tracerouteList)
{
	list<string> listCopy=tracerouteList;
	tracerouteList.sort();
	tracerouteList.unique();
	int listSize=tracerouteList.size();
	double total=listCopy.size();
	double Entropy=0;
	for(int i=0;i<listSize;i++)
	{
		double count=getListCount(listCopy,tracerouteList.front());
		Entropy+=((double)(count/total)*(double)(log10(count/total)));
		tracerouteList.pop_front();
	}
	Entropy*=-1;
	return Entropy;
}

