
#include "stdafx.h"
#include "ip.h"
#include <winsock.h>


using namespace common;
using namespace std;


// ���� ��ǻ���� IP Address�� �����Ѵ�.
string common::GetHostIP(const int networkCardIndex)
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		return "";
	}


	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) 
	{
		return "";
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) 
	{
		return "";
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i)
	{
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));

		if (networkCardIndex == i)
			return inet_ntoa(addr);
	}

	WSACleanup();

	return "";
}


// ���� ��ǻ���� ��� ��Ʈ��ũ ī���� IP Address�� �����Ѵ�.
void common::GetHostIPAll(vector<string> &ips)
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		return;
	}

	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR)
	{
		return;
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0)
	{
		return;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i)
	{
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		ips.push_back(inet_ntoa(addr));
	}

	WSACleanup();
}


uint32_t parseIPV4string(const string &ipAddress) 
{
	char ipbytes[4];
	sscanf(ipAddress.c_str(), "%uhh.%uhh.%uhh.%uhh", &ipbytes[3], &ipbytes[2], &ipbytes[1], &ipbytes[0]);
	return ipbytes[0] | ipbytes[1] << 8 | ipbytes[2] << 16 | ipbytes[3] << 24;
}
