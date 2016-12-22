#include <ctime>
#include <iostream>
#include <string>

#include "QueryASN.h" 
using namespace std;

int main()
{
    QueryASN q;
    string str= q.queryASN("140.116.177.110");
	cout << str << endl;
    return 0;
}
