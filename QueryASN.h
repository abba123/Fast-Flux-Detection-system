#ifndef QUERYASN_H
#define QUERYASN_H
#include <string>

using namespace std;
class QueryASN
{
    public:
        string queryASN(string ip);

    protected:

    private:
		string queryPage(string ip);
};

#endif // QUERYASN_H
