#pragma once


#include "MPrerequisites.h"
#include "MMemory.h"
#include "MString.h"


class MFileUtil
{
public:
	// 파일을 로드
	static MBOOL LoadFile(MMemory& inMemory, const MString& inPath);

	// 파일에 저장
	static MBOOL SaveToFile(MMemory& inMemory, const MString& inPath);
	static MBOOL SaveToFile(const void* inData, MSIZE inSize, const MString& inPath);

};

