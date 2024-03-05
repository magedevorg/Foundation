#pragma once

#include "MPrerequisites.h"
#include <memory.h>


// 메모리 관련 로직
class MMemoryAllocator
{
public:
    // 메모리 할당 로직
    template <typename T>
    static void Allocation(T*& outPointer, MSIZE& outCapacity, MSIZE inNewCapacity, T* inInlineMemory, MSIZE inInlineCapacity)
    {
        // 사이즈가 동일하다면 처리하지 않는다
        if( outCapacity == inNewCapacity) {
            return;
        }

        // 기존 메모리 해제
        MMemoryAllocator::Release<T>(outPointer, outCapacity, inInlineMemory);

        // 용량 설정
        outCapacity = inNewCapacity;

        // 할당할게 없다면 리턴
        if( 0 == inNewCapacity) {
            return;
        }

        // 인라인 버퍼보다 크다면 새로 할당
        if( inInlineCapacity < inNewCapacity )
        {
            outPointer = new T[inNewCapacity]; 
        }
        else
        {
            // 아닐경우 인라인 버퍼 사용
            outPointer = inInlineMemory;
        }
    }


    template <typename T>
    static void Reallocation(T*& inOutPointer, MSIZE& inOutCapacity, MSIZE inNewCapacity, T* inInlineMemory, MSIZE inInlineCapacity)
    {
        // 사이즈가 동일하다면 처리하지 않는다
        if( inOutCapacity == inNewCapacity) {
            return;
        }

        MSIZE prevSize = sizeof(T) * inOutCapacity;

        // 기존에 메모리가 있다면 복사해둔다
        T* prevMemory = nullptr;
        if( 0 < inOutCapacity) 
        { 
            // 메모리를 할당하고 값을 카피
            prevMemory = new T[inOutCapacity];
            ::memcpy(prevMemory, inOutPointer,  prevSize);
        }

        // 사용할 메모리 할당
        MMemoryAllocator::Allocation<T>(inOutPointer, inOutCapacity,inNewCapacity, inInlineMemory,inInlineCapacity);
        if( 0 < inNewCapacity)
        {
            // 신규 메모리에 카피
            if (0 < prevSize)
            {
                // 작은 값으로 복사
                MSIZE copySize = MMIN_VALUE(prevSize, inNewCapacity);
                ::memcpy(inOutPointer, prevMemory, copySize);
            }
        }

        MSAFE_DELETE_ARRAY(prevMemory);
    }

    // 메모리 해제 로직
    template <typename T>
    static void Release(T*& outPointer, MSIZE& outCapacity, T* inInlineMemory)
    {
        // 메모리가 할당되어있고 인라인 메모리가 아닌경우 메모리 해제
        if( nullptr != outPointer && outPointer != inInlineMemory)
        {
            MSAFE_DELETE_ARRAY(outPointer);
        }

        outPointer = nullptr;
        outCapacity = 0;
    }

    //-------------------------------------------------------------------
    // 일반 할당/해제
    //-------------------------------------------------------------------
    template <typename T>
    static T* Alloc(MSIZE inSize)
    {
        if (0 < inSize) {
            return new T[inSize];
        }
        return nullptr;
    }

    template <typename T>
    static void Release(T* inPointer)
    {
        if (nullptr != inPointer) {
            delete[] inPointer;
        }
    }

    //-------------------------------------------------------------------
    // 인라인이 있는 할당/해제
    //-------------------------------------------------------------------
    template <typename T>
    static T* Alloc(MSIZE inSize, T* inInlineMemory, MSIZE inInlineSize)
    {
        // 사이즈가 인라인 메모리 보다 크다면 새로 할당
        if (0 < inSize && inInlineSize < inSize) {
            return new T[inSize];
        }

        // 작은경우 인라인 메모리 사용
        return inInlineMemory;
    }

    template <typename T>
    static void Release(T* inPointer, T* inInlineMemory)
    {
        // 메모리가 할당되어있고 인라인 메모리가 아닌경우 메모리 해제
        if(nullptr != inPointer && inPointer != inInlineMemory ) {
            delete[] inPointer;
        }
    }
};