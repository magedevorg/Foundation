#include "MFileUtil.h"
#include <stdio.h>



MBOOL MFileUtil::LoadFile(MMemory& inMemory, const MString& inPath)
{
	FILE* file = _wfopen(inPath.GetStr(), MSTR("rb"));
	if (nullptr == file) {
		return MFALSE;
	}
	
	// ���� ����� ���Ѵ�
	fseek(file, 0, SEEK_END);
	const long fileSize = ftell(file);

	// �޸� �Ҵ�
	inMemory.Alloc(fileSize);

	// ���� ����� �ִٸ� �д´�
	if (0 < fileSize) 
	{
		fseek(file, 0, SEEK_SET);
		fread(inMemory.GetPointer(), fileSize, 1, file);
	}
	
	// ������ �ݴ´�
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

	// ������ �ݴ´�
	fclose(file);

	return MTRUE;
}