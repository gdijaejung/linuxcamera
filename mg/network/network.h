//
// 2016-02-06, jjuiddong
//
// gcc용 network 전역 선언
//
//
#pragma once


#include "../common/common.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

typedef int SOCKET;


namespace network {
	enum {
		BUFFER_LENGTH = 512,
	};

	struct fd_array : fd_set
	{
		SOCKET fd_array[128];
		int fd_count;
	};
}

namespace
{
	class __GET_TICK_COUNT
	{
	public:
		__GET_TICK_COUNT()
		{
			if (gettimeofday(&tv_, NULL) != 0)
				throw 0;
		}
		timeval tv_;
	};
	__GET_TICK_COUNT timeStart;
}

unsigned long GetTickCount();
unsigned long GetTickCount2();
std::string GetHostIP(const int networkCardIndex);


// #include "common.h"
// #include "autocs.h"
#include "session.h"
#include "packetqueue.h"
#include "tcpserver.h"
#include "tcpclient.h"
#include "udpclient.h"
#include "udpserver.h"
