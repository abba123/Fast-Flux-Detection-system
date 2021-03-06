#include <iostream>
#include "QueryASN.h"
#include <string.h>
#include <curl/curl.h>

using namespace std;
string QueryASN::queryASN(string ip)
{
	string pageResult = queryPage(ip);
	if (pageResult.compare("")==0)
	{
		cout<< ip << " : query asn failed" << endl;
		return "";
	}
	pageResult=pageResult.substr(pageResult.find("AS Name")+7);
	string result=pageResult.substr(1,pageResult.find("|")-3);
	return result;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

string QueryASN::queryPage(string ip)
{
	string result;
	char POST[255]="";
	CURL *curl;
	string postFields="family=ipv4&method_whois=whois&action=do_whois&bulk_paste=";
	postFields.append(ip);

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http://asn.cymru.com/cgi-bin/whois.cgi");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postFields.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	if(result.find("Error! Timeout (10s)")!=-1)	return "";

	int stringStart = result.find("[v4.whois.cymru.com]");
	int stringEnd=result.find("</PRE>");
	result=result.substr(stringStart,stringEnd-stringStart);
	return result;
}
