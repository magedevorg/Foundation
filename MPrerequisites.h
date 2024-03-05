#pragma once 

// 플랫폼
#define MPLATFORM_WINDOWS   1       // Windows
#define MPLATFORM_LINUX     2       // Linux
#define MPLATFORM_MAC       3       // Mac       


#ifdef _WIN32
#   define MPLATFORM MPLATFORM_WINDOWS
#elif __linux__
#   define MPLATFORM MPLATFORM_LINUX
#elif __APPLE__
#   include "TargetConditionals.h"
#   if TARGET_OS_MAC
#       define MPLATFORM MPLATFORM_MAC
#   endif
#endif


//---------------------------------------------------------------
// 기본 헤더 
//---------------------------------------------------------------
#include <list>
#include <vector>
#include <map>
#include <stack>
#include <array>




// 타입 재정의
#define MBOOL           bool
#define MTRUE           true
#define MFALSE          false

#define MINT16          short
#define MUINT16         unsigned short

#define MINT32          int
#define MUINT32         unsigned int

#define MINT64          long long
#define MUINT64         unsigned long long

#define MCHAR           char
#define MWCHAR          wchar_t

#define MBYTE           unsigned char
#define MSIZE           int

#define MFLOAT          float

#define MINDEX          int
#define MINDEX_NONE     -1

// 스트링 매크로
#define MSTR(IN_STR)       L##IN_STR

#define MSAFE_DELETE(PTR)           if(nullptr != PTR) {delete PTR; PTR = nullptr;}
#define MSAFE_DELETE_ARRAY(PTR)     if(nullptr != PTR) {delete[] PTR; PTR = nullptr;}


#define MMAX_VALUE(A, B) (A>B?A:B)
#define MMIN_VALUE(A, B) (A>B?B:A)


#define MCLEAR_PTR_VECTOR(VEC) {for(auto ITEM : VEC) {delete ITEM;} VEC.clear();}

#define MCLEAR_PTR_QUEUE(QUEUE)		\
{									\
	while(false == QUEUE.empty())	\
	{								\
		auto ITEM = QUEUE.front();	\
		delete ITEM;				\
		QUEUE.pop();				\
	}								\
}									\

#define MCLEAR_PTR_STACK(STACK)		\
{									\
	while (false == STACK.empty())	\
	{								\
		auto ITEM = STACK.top();	\
		delete ITEM;				\
		STACK.pop();				\
	}								\
}									
