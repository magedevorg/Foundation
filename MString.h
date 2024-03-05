#pragma once

#include "MPrerequisites.h"
#include "MStringUtil.h"


// 스트링 매크로(os에 따라서 리터럴을 u로 사용해야 할수도 있다)
#define MSTR(str) L##str



//--------------------------------------------------
// 기본 스트링 클래스
//--------------------------------------------------
class MString
{
public:
    MString() {}
    MString(const MWCHAR* inStr) {
        Copy(inStr);
    }

    MString(const MString& inOther) {
        Copy(inOther.GetStr());
    }

    virtual ~MString()
    {
        MMemoryAllocator::Release(Str);
    }

public:
    // 할당된 용량을 얻는다
    MSIZE GetSize() const {
        return Size;
    }

    //---------------------------------------
    // 연산자 오버로딩
    //---------------------------------------
    MString& operator=(const MWCHAR* inStr) 
    {
        Copy(inStr);
        return *this;
    }

    MString& operator=(const MString& inOther)
    {
        Copy(inOther.GetStr());
        return *this;
    }
    

    const MWCHAR* GetStr() const 
    {
        if (nullptr != Str) {
            return Str;
        }
        return MSTR("");
    }

    // 스트링 설정
    void SetStr(const void* inStr, EMCharset inCharSet);


    // 포멧 처리
    void Format(const MWCHAR* inFormat, ...);

protected:
    // 문자열 복사
    void Copy(const MWCHAR* inStr);

    // 메모리 할당
    void Alloc(MSIZE inSize);
    
protected:
    MWCHAR* Str = nullptr;
    MSIZE Size = 0;
};

