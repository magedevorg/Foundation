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