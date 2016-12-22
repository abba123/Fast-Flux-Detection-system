#include "ASNUtil.h"
#include <list>
#include<string>

using namespace std;

//
list<string> ASNUtil::findUniqueASN(list<string> asnList)
{
    asnList.unique();
    return asnList;
}


