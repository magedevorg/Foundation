#pragma once

#include "MPrerequisites.h"

//-----------------------------------------------------------
// 플래그 설정
//-----------------------------------------------------------
#if (MPLATFORM == MPLATFORM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN			// 불필요한 API 제외
#	define NOMINMAX						// min/max 매크로 사용 안함
#endif


//-----------------------------------------------------------
// 플랫폼별 해더
//-----------------------------------------------------------
// 리눅스 계열 공통 헤더
#if( MPLATFORM == MPLATFORM_LINUX || MPLATFORM == MPLATFORM_MAC)
#	include <unistd.h>
# 	include <arpa/inet.h>
#	include <sys/socket.h>
#   include <sys/ioctl.h>
#	include <netinet/in.h>
#endif


// 각 플랫폼 헤더
#if (MPLATFORM == MPLATFORM_WINDOWS)
#	include <WinSock2.h>
#   include <WS2tcpip.h>
#   include <MSWSock.h>
#	include <Windows.h>
#elif (MPLATFORM == MPLATFORM_LINUX )
#   include <sys/epoll.h>       // use epoll
#elif (MPLATFORM == MPLATFORM_MAC)
#   include <sys/time.h>
#   include <sys/event.h>
#   include <sys/types.h>
#endif




//-----------------------------------------------------------
// 후처리
//-----------------------------------------------------------
#if (MPLATFORM == MPLATFORM_WINDOWS)
	// 윈도우인경우 특정 매크로는 사용하지 않는다. 
	// (언리얼에서 해당 이름의 함수에서 에러 발생)
#   undef DeleteFile
#   undef UpdateResource
#   undef MoveFile
#   undef CopyFile
#   undef CreateDirectory

#endif