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
	if (MFALSE == MakeDirectory(inPath)) {
		return MFALSE;
	}

	FILE* file = _wfopen(inPath.GetStr(), MSTR("wb"));
	if (nullptr == file) {
		return MFALSE;
	}

	fwrite(inData, inSize, 1, file);

	// ������ �ݴ´�
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

	// ����� �ε���
	MINT32 index = 0;

	// ���� ���ڿ��� ���ö����� ����
	while (L'\0' != dir[index])
	{
		// ���͸� �������ΰ��
		if( L'\\' == dir[index] )
		{
			// üũ�� ���ؼ� �ش� ��ġ�� ���Ṯ�ڷ� ����
			dir.CopyTo(temp, 0, index + 1);
			
			// �ش� ��ο� ���͸� ����
			if (MFALSE == MakeDirInternal_WINDOWS((const WCHAR*)temp.GetPointer())) {
				return MFALSE;
			}
		}

		// �ε��� ����
		++index;
	}

	return MTRUE;
}



MBOOL MFileUtil::MakeDirInternal_WINDOWS(const MWCHAR* inPath)
{
	// ��� ���͸��� �ִ��� üũ�ϰ� ���ٸ� ����

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