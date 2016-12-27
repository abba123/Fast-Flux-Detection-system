#include <iostream>
#include <list>
#include <math.h>

#include "RTTUtil.h"

using namespace std;

double RTTUtil::maxRTT(list<double> RTT)
{
	RTT.sort();
	return RTT.back();
}

double RTTUtil::minRTT(list<double> RTT)
{
	RTT.sort();
	return RTT.front();
}

double RTTUtil::avgRTT(list<double> RTT)
{
	int size=RTT.size();
	double avg=0;
	for(int i=0;i<size;i++)
	{
		avg+=RTT.front();
		RTT.pop_front();
	}
	avg/=size;
	return avg;
}

double RTTUtil::stdRTT(list<double> RTT)
{
	double mean=avgRTT(RTT);
	double std=0;
	int size=RTT.size();
	for(int i=0;i<size;i++)
	{
		std+=pow((RTT.front()-mean),2);
		RTT.pop_front();
	}
	std/=size;
	std=sqrt(std);
	return std;
}
