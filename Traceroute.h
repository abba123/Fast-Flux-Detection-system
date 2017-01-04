#include <string>
#include <list>
using namespace std;
class Traceroute
{
    public:
        list<string> getTracerouteList(list<string> ipList);
		double TracerouteEntropy(list<string> TracerouteList);
	protected:

    private:
        list<string> getTraceroute(string ip);
		int checksum(unsigned short *buf,int sz);
		string getDomain(string ip);
		string sendTraceroutePkt(string ip, unsigned short ttl);
};
