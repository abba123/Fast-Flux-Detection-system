#include <iostream>
#include <string>
#include <ctime>
#include <list>

#include "ASNUtil.h"
#include "QueryASN.h"
#include "RTTQuery.h"
#include "RTTUtil.h"
#include "Traceroute.h"
using namespace std;

void printList(list<string> l)
{
	int size=l.size();
	for(int i=0;i<size;i++)
	{
		cout << l.front() << " ";
		l.pop_front();
	}
	cout << endl;
}

int main(int argc, char* argv[])
{
    // date MM/DD/YY
    time_t now = time(0);
    tm *t = localtime(&now);

    int year = 1900+t->tm_year;
    int month = 1+t->tm_mon;
    int day = t->tm_mday;
/////////////////////////////
	int best[5]={20,115,5,818,700};

    ASNUtil asnUtility;
	QueryASN asnQuery;
	RTTQuery rttQuery;
	RTTUtil rttUtility;
	Traceroute tr;
	
	string url = argv[1];
	cout << "start query ASN..." << endl;	
	list<string> ipList=asnUtility.getIPList(url);	//query A record
	list<string> asnList=asnUtility.getASNList(ipList);	//find asn
	asnList=asnUtility.findUniqueASN(asnList);	//get unique asn
	
	cout << "start get traceroute entropy..." << endl;	
	list<string> trList=tr.getTracerouteList(ipList);
	double trEntropy=tr.TracerouteEntropy(trList);

	cout << "start get rtt..." << endl;	
	list<double> rttList=rttQuery.getRTTList(ipList);
	double rttstd=rttUtility.stdRTT(rttList);

	cout << "A Record : " << ipList.size() << endl;
	cout << "Unique ASN : " << asnList.size() << endl;
	cout << "RTT STD : " << rttstd << endl;
	cout << "Traceroute Entropy : " << trEntropy << endl;

	double result=0;
	result = ipList.size()*best[0] + asnList.size()*best[1] + rttstd/100*best[2] + trEntropy*best[3] - best[4];
	if(result > 0)	cout << "FF Domain" << endl;
	else	cout << "Benign Domain" << endl;
	
	return 0;
	
}
