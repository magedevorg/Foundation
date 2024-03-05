#pragma once

#include "MMemoryAllocator.h"
#include "MStream.h"


//----------------------------------------------------------
// 버퍼 클래스
// 데이터가 순차적으로 들어가고 나간다
//----------------------------------------------------------
class MBuffer
{
public:
    MBuffer() {}
    MBuffer(MSIZE inSize) 
        : Size(inSize)
    {
        Pointer = MMemoryAllocator::Alloc<MBYTE>(inSize);
        Reset();
    }

    MBuffer(const MBuffer& inOther) = delete;
    
    virtual ~MBuffer() {
        MMemoryAllocator::Release<MBYTE>(Pointer);
    }

public:
    // 할당. 기존 정보는 제거.
    virtual void Alloc(MSIZE inSize, MBOOL inIgnoreSmall = MFALSE);
    
    // 메모리 추가
    virtual void Append(MSIZE inAppendSize);

    // 용량 체크
    MBOOL CheckSize(MSIZE inSize)
	{
		if (inSize < WritableSize) {
			return MTRUE;
		}
		return MFALSE;
	}

    // Push
    MBOOL Push(const void* inData, MSIZE inSize, MBOOL inAutoAppend = MFALSE);
    MBOOL Push(MSerializable& inData , MBOOL inAutoAppend = MFALSE);
    
    // Pop
    void Pop(MSIZE inSize);
    
    // 읽기
    void Read(void* outData, MSIZE inSize);
    

    // 버퍼 리셋
    void Reset() 
    {
        // 정보 갱신
        WritePos = 0;
        ReadPos = 0;

        WritableSize = Size;
        ReadableSize = 0;
    }

    // 읽기 가능한 사이즈를 얻어온다
    MSIZE GetReadableSize() const {
        return ReadableSize;
    }

    // 읽기 위치  포인터를 얻는다
    MBYTE* GetReadPointer() {
        return Pointer + ReadPos;
    }

    // 쓰기 가능사이즈를 얻는다
    MSIZE GetWritableSize() const {
        return WritableSize;
    }

    // 정보가 비었는지 체크
    MBOOL IsEmpty() const {
        return 0 == GetReadableSize();
    }

protected:
    // 메모리 정리
    void OrganizeMemory();


protected:
    // 메모리 정보
    MBYTE* Pointer = nullptr;
    MSIZE Size = 0;

    // 위치 정보
    MINDEX WritePos			= 0;		// 쓰기 위치
    MINDEX ReadPos          = 0;        // 읽은 위치

    // 사이즈 관련 정보
    MSIZE WritableSize		= 0;        // 쓰기 가능 사이즈   
    MSIZE ReadableSize      = 0;        // 읽기 가능 사이즈
};



//----------------------------------------------------------
// 인라인 메모리를 사용하는 버퍼 클래스
//----------------------------------------------------------
template <MSIZE INLINE_SIZE>
class MBufferI : public MBuffer
{
public:
    MBufferI() 
    {
        Size = INLINE_SIZE;
        Pointer = InlineMemory;
        Reset();
    }

    MBufferI(MSIZE inSize) 
    {
        Size = inSize;
        Pointer = MMemoryAllocator::Alloc<MBYTE>(Size, InlineMemory, INLINE_SIZE);
        Reset();
    }
    
    virtual ~MBufferI() {
        MMemoryAllocator::Release<MBYTE>(Pointer, InlineMemory);
        Pointer = nullptr;
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

        // 메모리 해제
        MMemoryAllocator::Release<MBYTE>(Pointer, InlineMemory);

        // 메모리 할당
        Pointer = MMemoryAllocator::Alloc<MBYTE>(inSize, InlineMemory, INLINE_SIZE);

        // 리셋
        Reset();
    }

    // 메모리 추가
    virtual void Append(MSIZE inAppendSize) override
    {
        if (inAppendSize <= 0) {
            return;
        }

        OrganizeMemory();

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

        // 쓰기 가능 사이즈 증가
        WritableSize += inAppendSize;
    }

protected:
    // 인라인 버퍼
    MBYTE InlineMemory[INLINE_SIZE] = {0,};
};


//---------------------------------------------------------------------
// 버퍼 스트림
//---------------------------------------------------------------------
class MBufferStream : public MStream
{
public:
	MBufferStream(MBuffer& inBuffer)
		: Buffer(inBuffer)
	{}
    
protected:
    MBuffer& Buffer;
};

// 읽기 스트림
class MBufferReadStream : public MBufferStream
{
public:
	MBufferReadStream(MBuffer& inBuffer)
		: MBufferStream(inBuffer)
	{}

public:
    virtual MBOOL IsRead() override {
        return MTRUE;
    }

protected:
    virtual void ProcessData(void* inData, MSIZE inSize) override 
    {
        Buffer.Read(inData, inSize);
        Buffer.Pop(inSize);
    }
};

// 쓰기 스트림
class MBufferWriteStream : public MBufferStream
{
public:
    MBufferWriteStream(MBuffer& inBuffer)
        : MBufferStream(inBuffer)
    {}
        
public:
    virtual MBOOL IsRead() override {
        return MFALSE;
    }

protected:
    virtual void ProcessData(void* inData, MSIZE inSize) override 
    {
        Buffer.Push(inData, inSize);
    }
};