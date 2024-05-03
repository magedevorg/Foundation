#include "MFileUtil.h"
#include "MArray.h"
#include "MPath.h"

#include <stdio.h>






MBOOL MFileUtil::LoadFile(MMemory& inMemory, const MString& inPath)
{
	FILE* file = _wfopen(inPath.GetStr(), MSTR("rb"));
	if (nullptr == file) {
		return MFALSE;
	}
	
	// 파일 사이즈를 구한다
	fseek(file, 0, SEEK_END);
	const long fileSize = ftell(file);

	// 메모리 할당
	inMemory.Alloc(fileSize);

	// 파일 사이즈가 있다면 읽는다
	if (0 < fileSize) 
	{
		fseek(file, 0, SEEK_SET);
		fread(inMemory.GetPointer(), fileSize, 1, file);
	}
	
	// 파일을 닫는다
	fclose(file);

	return MTRUE;
}



MBOOL MFileUtil::SaveToFile(MMemory& inMemory, const MString& inPath)
{
	return SaveToFile(inMemory.GetPointer(), inMemory.GetSize(), inPath);
}


MBOOL MFileUtil::SaveToFile(const void* inData, MSIZE inSize, const MString& inPath)
{
	if (MFALSE == MakeDirectory(inPath)) {
		return MFALSE;
	}

	FILE* file = _wfopen(inPath.GetStr(), MSTR("wb"));
	if (nullptr == file) {
		return MFALSE;
	}

	fwrite(inData, inSize, 1, file);

	// 파일을 닫는다
	fclose(file);

	return MTRUE;
}



#if (MPLATFORM == MPLATFORM_WINDOWS)
MBOOL MFileUtil::MakeDirectory(const MString& inPath)
{
	MPath path(inPath);

	MString dir = path.GetDirectory();
	if (0 == dir.GetLength()) {
		return MTRUE;
	}

	MMemoryI<256> temp;

	// 사용할 인덱스
	MINT32 index = 0;

	// 종료 문자열이 나올때까지 루프
	while (L'\0' != dir[index])
	{
		// 디렉터리 구분자인경우
		if( L'\\' == dir[index] )
		{
			// 체크를 위해서 해당 위치를 종료문자로 변경
			dir.CopyTo(temp, 0, index + 1);
			
			// 해당 경로에 디렉터리 생성
			if (MFALSE == MakeDirInternal_WINDOWS((const WCHAR*)temp.GetPointer())) {
				return MFALSE;
			}
		}

		// 인덱스 증가
		++index;
	}

	return MTRUE;
}



MBOOL MFileUtil::MakeDirInternal_WINDOWS(const MWCHAR* inPath)
{
	// 대상 디렉터리가 있는지 체크하고 없다면 생성

	ULONG attr = GetFileAttributesW(inPath);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		return (::CreateDirectoryW(inPath, NULL) == TRUE) ? MTRUE : MFALSE;
	} 
	
	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		return MTRUE; 

	return MFALSE;
}

#endif