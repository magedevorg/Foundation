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
	// 디렉터리를 분리



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
	// 초기 메모리 사이즈
	const MINT32 INIT_COUNT = 3;

	// 경로 관련은 splitpath사용

	// 메모리
	MMemoryI<(INIT_COUNT + 2) * sizeof(WCHAR)> tempMemory;
	tempMemory.Clear();

	// 전체 경로로 만든다
	{
		DWORD count = ::GetFullPathNameW(inPath.GetStr(), INIT_COUNT, (WCHAR*)tempMemory.GetPointer(), NULL);

		// 메모리가 부족한경우 재할당 해서 다시한번 처리
		if (INIT_COUNT < count)
		{
			tempMemory.Alloc((count + 2) * sizeof(WCHAR));
			tempMemory.Clear();

			// 다시 처리
			count = ::GetFullPathNameW(inPath.GetStr(), count, (WCHAR*)(tempMemory.GetPointer()), NULL);
		}

		if (0 == count)
		{
			// 실패
			return MFALSE;
		}

		// 토탈 경로가 정상적으로 생성되었다면 마지막이 '\\' 인지 체크하고 아니라면 설정
		WCHAR lastChar = ((WCHAR*)tempMemory.GetPointer())[count - 1];
		if ('\\' != lastChar)
		{
			((WCHAR*)tempMemory.GetPointer())[count] = '\\';
		}
	}
	
	// 성공했다고 치고
	WCHAR* fullPath = (WCHAR*)(tempMemory.GetPointer());
	
	// 사용할 인덱스
	MINT32 index = 0;

	// 종료 문자열이 나올때까지 루프
	while (L'\0' != fullPath[index])
	{
		// 디렉터리 구분자인경우
		if( L'\\' == fullPath[index] )
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