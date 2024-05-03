#include "MFileUtil.h"
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
	FILE* file = _wfopen(inPath.GetStr(), MSTR("wb"));
	if (nullptr == file) {
		return MFALSE;
	}

	fwrite(inData, inSize, 1, file);

	// 파일을 닫는다
	fclose(file);

	return MTRUE;
}



MBOOL MFileUtil::MakeFile(const MString& inPath)
{
#if (MPLATFORM == MPLATFORM_WINDOWS )
	{
		
		//SHCreateDirectory
	}
#endif

	return MFALSE;
}


MBOOL MFileUtil::MakeDirectory(const MString& inPath)
{
#if (MPLATFORM == MPLATFORM_WINDOWS)
	return MFileUtil::MakeDir_WINDOWS(inPath);
#endif

	return MFALSE;
}



#if (MPLATFORM == MPLATFORM_WINDOWS)
MBOOL MFileUtil::MakeDir_WINDOWS(const MString& inPath)
{
	MMemoryI<512> tempMemory;

	// 전체 경로로 만든다
	GetFullPathNameW(inPath.GetStr(), inPath.GetAllocSize(), (WCHAR*)(tempMemory.GetPointer()), NULL);
	
	// 여기서 경로를 못만들었을때에 대한 처리가 필요

	// 성공했다고 치고
	WCHAR* fullPath = (WCHAR*)(tempMemory.GetPointer());
	
	// 사용할 인덱스
	MINT32 index = 0;

	// 종료 문자열이 나올때까지 루프
	while (L'\0' != fullPath[index])
	{
		// 디렉터리 구분자인경우
		while (L'\\' == fullPath[index])
		{
			// 체크를 위해서 해당 위치를 종료문자로 변경
			fullPath[index] = L'\0';
			
			// 해당 경로에 디렉터리 생성
			if (MFALSE == MakeDirInternal_WINDOWS(fullPath)) {
				return MFALSE;
			}

			// 복구
			fullPath[index] = L'\\';
		}
	}

	return MTRUE;
}



MBOOL MFileUtil::MakeDirInternal_WINDOWS(const MWCHAR* inPath)
{
	// 대상 디렉터리가 있는지 체크하고 없다면 생성

	ULONG attr = GetFileAttributesW(inPath);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		return (CreateDirectoryW(inPath, NULL) == TRUE) ? MTRUE : MFALSE;
	} 
	
	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		return MTRUE; 

	return MFALSE;
}

#endif