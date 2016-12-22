#include <iostream>
#include <ctime>
#include <list>

#include "ASNUtil.h"

using namespace std;

int main()
{
    // date MM/DD/YY
    time_t now = time(0);
    tm *t = localtime(&now);

    // year 1900+t->tm_year
    // month 1+t->tm_mon
    // day t->tm_mday


/////////////////////////////
    list<string> a;
    a.push_back("a");
    ASNUtil asn;
    a = asn.findUniqueASN(a);

}
