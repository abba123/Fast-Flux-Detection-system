#include <string>
#include <list>
using namespace std;
class Traceroute
{
    public:
        list<string> getTraceroute(string ip);
		double TracerouteEntropy(list<string> TracerouteList);
	protected:

    private:
		int checksum(unsigned short *buf,int sz);
		string getDomain(string ip);
		string sendTraceroutePkt(string ip, unsigned short ttl);
};
