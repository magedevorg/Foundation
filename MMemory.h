#pragma once

#include "MMemoryAllocator.h"
#include "MStream.h"
#include "MTrace.h"


//-------------------------------------------------------------
// 메모리 클래스
// 대입에 대한 처리는 막는다
//-------------------------------------------------------------
class MMemory : public MSerializable
{
public:
    MMemory() {}
    MMemory(MSIZE inSize) 
        : Size(inSize)
    {
        // 사이즈가 설정되었다면 메모리 할당
        if (0 < inSize) {
            Pointer = MMemoryAllocator::Alloc<MBYTE>(inSize);
        }
    }

    // 대입 생성자는 방지
    MMemory(const MMemory& inOther) = delete;

    // 제거
    virtual ~MMemory() {
        MMemoryAllocator::Release(Pointer);
    }

public:
    // 할당 로직. 기존 메모리는 제거.
    virtual void Alloc(MSIZE inSize, MBOOL inIgnoreSmall = MFALSE)
    {
        if (inSize == Size) {
            return;
        }

        // 작은 사이즈를 무시하는경우 사이즈 체크
        if (MTRUE == inIgnoreSmall && inSize < Size ) {
            return;    
        }

        // 기존 포인터 제거
        MMemoryAllocator::Release(Pointer);

        // 사이즈가 설정된경우 메모리 할당
        Size = inSize;
        if (0 < Size) {
            Pointer = MMemoryAllocator::Alloc<MBYTE>(inSize);
        }
    }

    virtual void Append(MSIZE inAppendSize)
    {
        if (inAppendSize <= 0) {
            return;
        }

        // 사이즈 체크
        const MSIZE newSize = Size + inAppendSize;
        if (0 == newSize) {
            return;
        }
        
        // 버퍼 새로 할당
        MBYTE* newPointer = MMemoryAllocator::Alloc<MBYTE>(newSize);

        // 기존에 메모리가 있었다면 복사하고 제거
        if (nullptr != Pointer) 
        {
            ::memcpy(newPointer, Pointer, Size);
            MMemoryAllocator::Release(Pointer);
        }
        
        // 신규 정보로 설정
        Pointer = newPointer;
        Size = newSize;
    }

    // 읽기
    MBOOL Read(void* outDest, MINDEX inReadPos, MSIZE inSize) 
    {
        if (Size < inReadPos + inSize)
        {
            MASSERT(MFALSE);
            return MFALSE;
        }
        ::memcpy((MBYTE*)outDest, Pointer + inReadPos, inSize);
        return MTRUE;
    }


    // 쓰기
    MBOOL Write(MINDEX inWritePos, const void* inSrc, MSIZE inSize, MBOOL inAutoAppend = MFALSE) 
    {
        const MSIZE lackMemory = (inWritePos + inSize) - Size;
        if (0 < lackMemory) 
        {
            if (MTRUE == inAutoAppend)
            {
                Append(lackMemory);
            }
            else
            {
                MASSERT(MFALSE);
                return MFALSE;
            }
        }

        ::memcpy(Pointer + inWritePos, (const MBYTE*)inSrc, inSize);
        return MTRUE;
    }

    MBYTE* GetPointer() const {
        return Pointer;
    }

    MSIZE GetSize() const {
        return Size;
    }

    void Clear()
    {
        if (nullptr != Pointer) {
            ::memset(Pointer, 0, Size);
        }
    }

public:
    // 대입 연산자 방지
    void operator=(const MMemory& inOther) = delete;

    // 직렬화/역직렬화
    virtual void Serialize(MStream& inStream) override
    {
        if (MTRUE == inStream.IsRead())
        {
            // 읽기일 경우 사이즈를 얻고 메모리를 할당
            MSIZE tempSize = 0;
            inStream.Process(&tempSize, sizeof(tempSize));

            // 메모리 할당
            Alloc(tempSize);
        }
        else
        {
            // 쓰기일 경우 사이즈 write
            inStream.Process(&Size, sizeof(Size));
        }

        // 처리
        inStream.Process(Pointer, Size);
    }

protected:
    MBYTE* Pointer = nullptr;
    MSIZE Size = 0;
};



//-------------------------------------------------------------
// 인라인 메모리 클래스
// 설정된 메모리를 넘어가는 경우 힙할당
//-------------------------------------------------------------
template <MSIZE INLINE_SIZE>
class MMemoryI : public MMemory
{
public:
    MMemoryI()
    {
        Size = INLINE_SIZE;
        Pointer = InlineMemory;
    }
    
    MMemoryI(MSIZE inSize) 
    {
        Size = inSize;
        Pointer = MMemoryAllocator::Alloc<MBYTE>(Size, InlineMemory, INLINE_SIZE);
    }

    virtual ~MMemoryI() 
    {
        MMemoryAllocator::Release<MBYTE>(Pointer, InlineMemory);
        Pointer = nullptr;  // 이거 설정해줘야 상위에서 삭제 로직 안탐
    }

public:
    virtual void Alloc(MSIZE inSize, MBOOL inIgnoreSmall = MFALSE) override
    {
        if (inSize == Size) {
            return;
        }

        // 작은 사이즈를 무시하는경우 사이즈 체크
        if (MTRUE == inIgnoreSmall && inSize < Size) {
            return;
        }

        // 기존 메모리 제거
        MMemoryAllocator::Release<MBYTE>(Pointer, InlineMemory);

        // 사이즈 할당
        Size = inSize;
        Pointer = MMemoryAllocator::Alloc<MBYTE>(Size, InlineMemory, INLINE_SIZE);
    }

    virtual void Append(MSIZE inAppendSize)
    {
        if (inAppendSize <= 0) {
            return;
        }

        // 추가되는 사이즈가 인라인 사이즈보다 작다면 처리할필요 없다
        const MSIZE newSize = Size + inAppendSize;
        if (INLINE_SIZE < newSize)
        {
            // 새로 메모리를 할당
            MBYTE* newPointer = MMemoryAllocator::Alloc<MBYTE>(newSize, InlineMemory, INLINE_SIZE);

            // 기존 포인터가 있었다면 복사
            if (nullptr != Pointer) {
                ::memcpy(newPointer, Pointer, Size);
            }

            // 기존 메모리 제거
            MMemoryAllocator::Release<MBYTE>(Pointer, InlineMemory);

            // 새로 정보 설정
            Pointer = newPointer;
        }

        // 사이즈 재설정
        Size = newSize;
    }

protected:
    // 인라인 버퍼
    MBYTE InlineMemory[INLINE_SIZE] = {0,};
};





//-------------------------------------------------------------
// 메모리 스트림
// - 메모리를 버퍼처럼 순차 처리
// - 하나의 인터페이스로 상황에 맞게 read/write 처리
//-------------------------------------------------------------
class MMemoryStream : public MStream
{
public:
    MMemoryStream(MMemory& inMemory) 
        : Memory(inMemory)
    {}

public:
    void Reset() {
        Pos = 0;
    }

    MINDEX GetPos() const {
        return Pos;
    }

protected:
    // 메모리
    MMemory& Memory;
    
    // 위치
    MINDEX Pos = 0;
};


//-------------------------------------------------------------
// 읽기 스트림
// - 메모리에서 데이터를 읽는다
//-------------------------------------------------------------
class MMemoryReadStream : public MMemoryStream
{
public:
    MMemoryReadStream(MMemory& inMemory)
        : MMemoryStream(inMemory)
        {}

public:
    virtual MBOOL IsRead() override {
        return MTRUE;
    }


protected:
    // 처리 인터페이스
    virtual void ProcessData(void* inData, MSIZE inSize) override
    {
        Memory.Read(inData, Pos, inSize);
        Pos += inSize;
    }
};



//-------------------------------------------------------------
// 쓰기 스트림
// - 메모리에 데이터를 쓴다
//-------------------------------------------------------------
class MMemoryWriteStream : public MMemoryStream
{
public:
    MMemoryWriteStream(MMemory& inMemory)
        : MMemoryStream(inMemory)
    {}

public:
    virtual MBOOL IsRead() override {
        return MFALSE;
    }

protected:
    // 처리 인터페이스
    virtual void ProcessData(void* inData, MSIZE inSize) override
    {
        // 메모리가 부족하면 자동으로 추가
        Memory.Write(Pos, inData, inSize, MTRUE);
        Pos += inSize;
    }
};