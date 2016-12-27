#include <string>
#include <list>
using namespace std;
class RTTQuery
{
    public:
        list<double> getRTTList(list<string> ipList);
	protected:

    private:
		double getRTT(string ip);
		int checksum(unsigned short *buf,int sz);
		double diff_in_ms(struct timespec t1, struct timespec t2);

};
