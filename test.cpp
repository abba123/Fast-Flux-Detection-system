#include <string.h>
#include <iostream>
#include <math.h>

#include "RTTQuery.h"
#include "ASNUtil.h"
#include "QueryASN.h"
#include "RTTUtil.h"
#include "Traceroute.h"

using namespace std;

void printlist(list<string> l)
{
	int size=l.size();
	for(int i=0;i<size;i++)
	{
		cout << l.front() <<endl;
		l.pop_front();
	}
}

int main ()
{
		Traceroute t;
		ASNUtil aa;
		//list<string> b=aa.getIPList("groovenotes.org");
		//list<string> b=aa.getIPList("metric.nationalgeographic.com");
		//list<string> b=aa.getIPList("yahoo.com.tw");
		list<string> b=aa.getIPList("yahoo.com");
		int size=b.size();
		list<string> a;
		for(int i=0;i<size;i++)
		{
			list<string> tmp = t.getTraceroute(b.front());
			a.splice(a.end(),tmp);
			b.pop_front();
		}
		cout << t.TracerouteEntropy(a)<<endl;

}
