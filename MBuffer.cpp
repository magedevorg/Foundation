#include "MBuffer.h"
#include "MStream.h"
#include "MMemory.h"

#include <memory.h>



void MBuffer::Alloc(MSIZE inSize, MBOOL inIgnoreSmall)
{
    if (inSize == Size) {
        return;
    }

    // 작은 사이즈를 무시하는경우 사이즈 체크
    if (MTRUE == inIgnoreSmall && inSize < Size) {
        return;
    }

    // 메모리 해제
    MMemoryAllocator::Release(Pointer);

    // 메모리 할당
    Pointer = MMemoryAllocator::Alloc<MBYTE>(inSize);
    Size = inSize;

    // 리셋
    Reset();
}


void MBuffer::Append(MSIZE inAppendSize)
{
    if (inAppendSize <= 0) {
        return;
    }

    OrganizeMemory();

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
        MMemoryAllocator::Release<MBYTE>(Pointer);
    }

    // 신규 정보로 설정
    Pointer = newPointer;
    Size = newSize;

    // 쓰기 가능 사이즈 증가
    WritableSize += inAppendSize;
}


MBOOL MBuffer::Push(const void* inData, MSIZE inSize, MBOOL inAutoAppend)
{
    OrganizeMemory();

    // 사이즈가 적다면 실패
    const MSIZE lackMemory = inSize - WritableSize;
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
    
    // 값을 저장
    ::memcpy(Pointer + WritePos, inData, inSize);

    // 쓰기 위치 갱신
    WritePos += inSize;

    // 쓰기 가능한 사이즈 감소
    WritableSize -= inSize;

    // 읽기 가능한 사이즈 증가
    ReadableSize += inSize;

    return MTRUE;
}


MBOOL MBuffer::Push(MSerializable& inData, MBOOL inAutoAppend)
{
    MMemoryI<128> temp;
    MMemoryWriteStream writeStream(temp);
    writeStream.Process(inData);

    return MBuffer::Push(temp.GetPointer(), writeStream.GetPos(), inAutoAppend);
}


void MBuffer::Pop(MSIZE inSize)
{
    // 읽기 위치 갱신
    ReadPos += inSize;

    // 읽기 사이즈 감소
    ReadableSize -= inSize;

    // 쓰기 사이즈 증가
    WritableSize += inSize;
}


void MBuffer::Read(void* outData, MSIZE inSize)
{
    // 읽기 가능한 사이즈보다 더 많은 사이즈를 Pop하면 에러
    if( ReadableSize < inSize )
    {
        return;
    }

    // 메모리 카피
    ::memcpy(outData, Pointer + ReadPos, inSize);
}


void MBuffer::OrganizeMemory()
{
	// 쓰기전에 읽은 값이 있는지 체크
    // 읽은 값이 있다면 읽은 만큼 메모리 정보를 앞으로 이동하고 
	// 쓰기/읽기 위치를 갱신
    if (0 == ReadPos) {
        return;
    }
    
    // 읽을 사이즈가 있다면 앞으로 이동
    if (0 < ReadableSize)
    {
        // 여기 최적화 필요
        ::memmove(Pointer, Pointer + ReadPos, ReadableSize);
    }

    WritePos = ReadableSize;
    ReadPos = 0;
}



