#include "MString.h"
#include "memory.h"
#include <stdarg.h>


MString MString::operator+(const MString& inOther) const
{
	return *this + inOther.GetStr();
}

MString MString::operator+(const MWCHAR* inStr) const
{
	MString result(*this);
	result += inStr;
	return result;
}


const MString& MString::operator+=(const MWCHAR* inStr)
{
	// 추가할 스트링 길이를 얻는다(없다면 리턴)
	const MSIZE addLength = MStringUtil::GetLength(inStr);
	if (0 == addLength) {
		return *this;
	}

	// 신규 스트링 길이
	const MSIZE newLength = Length + addLength;

	// 신규 할당 사이즈
	const MSIZE newAllocSize = sizeof(MWCHAR) * (newLength + 1);
	
	// 신규 메모리 할당
	MBYTE* newBuffer = MMemoryAllocator::Alloc<MBYTE>(newAllocSize);
	::memset(newBuffer, 0, newAllocSize);
	
	// 기존 스트링 카피
	MSIZE prevSize = sizeof(MWCHAR) * Length;
	if (0 < prevSize) {
		::memcpy(newBuffer, Str, prevSize);
	}

	// 추가 스트링 카피(위에서 체크했기때문에 addLength는 체크할필요 없음)
	::memcpy(newBuffer + prevSize, inStr, sizeof(MWCHAR) * addLength);


	// 마무리
	{
		// 기존 스트링 해제
		MMemoryAllocator::Release(Str);

		// 신규 정보 설정
		Str = (MWCHAR*)newBuffer;
		Length = newLength;
		AllocSize = newAllocSize;
	}

	return *this;
}


bool MString::operator<(const MString& inStr) const
{
	return ::wcscmp(Str, inStr.Str) < 0;
}


void MString::Copy(const MWCHAR* inStr)
{
	const MSIZE newLength = MStringUtil::GetLength(inStr);

	CopyToCount(inStr, newLength);
}

void MString::CopyToCount(const MWCHAR* inStr, MINT32 inCount)
{
	// 길이를 체크해서 길이가 다르다면 재할당 처리
	if (inCount != Length)
	{
		// 기존 메모리를 해제하고 길이가 있다면 할당
		MMemoryAllocator::Release(Str);
		Str = nullptr;

		// 길이 설정
		Length = inCount;

		// 할당 사이즈 설정
		AllocSize = sizeof(MWCHAR) * (inCount + 1);

		// 할당할 사이즈가 있다면 할당
		if (0 < AllocSize) {
			Str = (MWCHAR*)MMemoryAllocator::Alloc<MBYTE>(AllocSize);
		}
	}

	// 정보가 있다면 카피
	if (0 < AllocSize) {
		::memcpy(Str, inStr, AllocSize);
	}
}

void MString::CopyTo(MMemory& inMemory, MINT32 inStartIndex, MINT32 inCount)
{
	inMemory.Alloc(sizeof(MWCHAR) * (inCount + 1), MTRUE);
	inMemory.Clear();

	::memcpy(inMemory.GetPointer(), &(Str[inStartIndex]), sizeof(MWCHAR) * inCount);
}



/*
void MString::Alloc(MSIZE inSize)
{
	// 사이즈가 동일하다면 처리하지 않는다
	if (AllocSize == inSize) {
		return;
	}

	AllocSize = inSize;

	// 해제
	MMemoryAllocator::Release(Str);
	Str = (MWCHAR*)MMemoryAllocator::Alloc<MBYTE>(inSize);
}
*/


void MString::SetStr(const void* inStr, EMCharset inCharSet)
{
	MMemoryI<256> temp;
	MStringUtil::Convert(temp, EMCharset::WCHAR, inStr, inCharSet);

	Copy((MWCHAR*)temp.GetPointer());
}


MString MString::Format(const MWCHAR* inFormat, ...)
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
			return (const MWCHAR*)tempMemory.GetPointer();
		}
	}

	return MSTR("");
}
