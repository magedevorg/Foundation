#include "MString.h"
#include "memory.h"
#include <stdarg.h>




void MString::Copy(const MWCHAR* inStr)
{
	const MSIZE size = MStringUtil::GetSize(EMCharset::WCHAR, inStr);
	
	// 메모리 할당
	Alloc(size);

	if (0 < size) {
		::memcpy(Str, inStr, size);
	}
}


void MString::Alloc(MSIZE inSize)
{
	// 사이즈가 동일하다면 처리하지 않는다
	if (Size == inSize) {
		return;
	}

	Size = inSize;

	// 해제
	MMemoryAllocator::Release(Str);
	Str = (MWCHAR*)MMemoryAllocator::Alloc<MBYTE>(inSize);
}


void MString::SetStr(const void* inStr, EMCharset inCharSet)
{
	MMemoryI<256> temp;
	MStringUtil::Convert(temp, EMCharset::WCHAR, inStr, inCharSet);

	Copy((MWCHAR*)temp.GetPointer());
}


void  MString::Format(const MWCHAR* inFormat, ...)
{
	const MSIZE addSize = 256;
	MMemoryI<addSize> tempMemory;

	while (true)
	{
		// 용량
		const MSIZE tempCapacity = tempMemory.GetSize();

		va_list ap;
		va_start(ap, inFormat);
		int result = vswprintf((MWCHAR*)tempMemory.GetPointer(), tempCapacity, inFormat, ap);
		va_end(ap);


		if (-1 == result)
		{
			// 성공하지 못한경우 재할당후 다시 처리
			tempMemory.Alloc(tempCapacity + addSize);
		}
		else
		{
			// 성공했다면 설정후 탈출한다
			Copy((const MWCHAR*)tempMemory.GetPointer());
			break;
		}
	}
}
