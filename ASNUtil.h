#ifndef ASNUTIL_H
#define ASNUTIL_H
#include <list>
#include <string>

using namespace std;

class ASNUtil
{
    public:
        list<string> findUniqueASN(list<string> asnList);
		list<string> getASNList(list<string> ip);
		list<string> getIPList(string url);
    protected:

    private:
};

#endif // ASNUTIL_H
