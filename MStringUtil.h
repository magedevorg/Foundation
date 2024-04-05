#pragma once


#include "MMemory.h"
#include "MError.h"
#include <array>
#include <wchar.h>



#define MSTRING_CONVERT_WINAPI          1
#define MSTRING_CONVERT_ICONV           2

#if (MPLATFORM == MPLATFORM_WINDOWS)      // 윈도우 플랫폼
#   define MSTRING_CONVERT MSTRING_CONVERT_WINAPI
#   define MCHAR_SET_ID   MUINT32
#else
#   define MSTRING_CONVERT MSTRING_CONVERT_ICONV
#   define MCHAR_SET_ID   const char*
#endif


//---------------------------------------------------------
// 코드 페이지
//---------------------------------------------------------
enum class EMCharset
{
    None = 0,
    UTF8 = 1,
    WCHAR = 2,
    Max,
};



//---------------------------------------------------------
// 스트링 전환
//---------------------------------------------------------
class MStringUtil
{
public:
    // 인코딩 변경
    static EMError Convert(MMemory& outDest, EMCharset inDestCharset, const void* inSrc, EMCharset inSrcCharset);

    // 스트링의 사이즈을 구한다(종료 문자 포함)
    static MSIZE GetSize(EMCharset inCharset, const void* inStr);

    // 스트링 길이를 얻는다
    static MSIZE GetLength(const MWCHAR* inStr);

    // 할당된 사이즈를 얻는다
    static MSIZE GetAllocSize(const MWCHAR* inStr);

 
protected:
    // 타입으로 케릭터 셋 아이디를 얻는다
    static MCHAR_SET_ID GetIDByCharset(EMCharset inCharset);

    // 캐릭터 셋 아이디 리스트
    static std::array<MCHAR_SET_ID, (MSIZE)EMCharset::Max> CharSetIDList;
};