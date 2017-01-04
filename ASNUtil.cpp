#include "ASNUtil.h"
#include "QueryASN.h"
#include <list>
#include <string>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

//
list<string> ASNUtil::findUniqueASN(list<string> asnList)
{
    asnList.sort();
	asnList.unique();
    return asnList;
}

list<string> ASNUtil::getASNList(list<string> ip)
{
	list<string> asnList;
	QueryASN query;
	int listSize=ip.size();
	for(int i=0;i<listSize;i++)
	{
		asnList.push_back(query.queryASN(ip.front()));
		ip.pop_front();
	}
	return asnList;
}

list<string> ASNUtil::getIPList(string url)
{
	list<string> ipList;
	struct hostent *record;
	record=gethostbyname(url.c_str());
	int i=0;
	while(record->h_addr_list[i]!=0)
	{
		in_addr* address=(in_addr*)record->h_addr_list[i];
		string ipAddress = inet_ntoa(* address);
		ipList.push_back(ipAddress);
		i++;
	}
		return ipList;
}
