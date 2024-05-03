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


	// 디렉토리를 생성
	static MBOOL MakeDirectory(const MString& inPath);

private:

#if (MPLATFORM == MPLATFORM_WINDOWS)
	// 디렉터리가 있는지 체크하고 없다면 생성
	static MBOOL MakeDir_WINDOWS(const MString& inPath);

	// 디렉터리 생성
	static MBOOL MakeDirInternal_WINDOWS(const MWCHAR* inPath);
#endif

};

