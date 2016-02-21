//
// 2016.02.16, jjuiddong
//
// linux용 common library
//
#pragma once


// #ifndef _CRT_SECURE_NO_WARNINGS
// 	#define _CRT_SECURE_NO_WARNINGS
// #endif



// 매크로 정의
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if (p) { delete p; p=NULL;} }
#endif
#ifndef SAFE_DELETEA
#define SAFE_DELETEA(p) {if (p) { delete[] p; p=NULL;} }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if (p) { p->Release(); p=NULL;} }
#endif
#ifndef SAFE_RELEASE2
#define SAFE_RELEASE2(p) {if (p) { p->release(); p=NULL;} }
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#define RET(exp)		{if((exp)) return; }			// exp가 true이면 리턴
#define RETV(exp,val)	{if((exp)) return val; }
#define ASSERT_RET(exp)		{assert(exp); RET(!(exp) ); }
#define ASSERT_RETV(exp,val)		{assert(exp); RETV(!(exp),val ); }
#define BRK(exp)		{if((exp)) break; }			// exp가 break

#ifndef MIN
	#define MIN(a,b) (((a)<(b))?(a):(b))
#endif 

#ifndef MAX
	#define MAX(a,b) (((a)>(b))?(a):(b))
#endif


#ifndef ZeroMemory
	#define ZeroMemory bzero
#endif

#ifndef BOOL
	#define BOOL bool
#endif

#ifndef TRUE
	#define TRUE true
#endif

#ifndef FALSE
	#define FALSE false
#endif

#ifndef NULL
	#define NULL (\0)
#endif


typedef unsigned int    u_int;
typedef unsigned char BYTE;
typedef unsigned int DWORD;


// 포함 파일들.

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> 
//#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>
//#include <arpa/inet.h>

#include <string>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <set>
//#include <boost/foreach.hpp>

using std::string;
using std::wstring;
using std::vector;
using std::map;
using std::list;
using std::set;
using std::stringstream;
using std::wstringstream;


#include "container/vectorhelper.h"
#include "math/Math.h"
#include "etc/singleton.h"
// #include "etc/filepath.h"
#include "etc/stringfunc.h"
// #include "etc/dbg.h"
#include "etc/observer.h"
#include "etc/observer2.h"
// #include "etc/rect.h"
#include "etc/genid.h"
// #include "etc/memorypool.h"
#include "etc/rand.h"
#include "etc/config.h"
#include "etc/autocs.h"
// #include "etc/Serial.h"
// #include "etc/BufferedSerial.h"
// #include "etc/SerialAsync.h"
// #include "etc/ip.h"
// 
