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

	// ��ü ��η� �����
	GetFullPathNameW(inPath.GetStr(), inPath.GetAllocSize(), (WCHAR*)(tempMemory.GetPointer()), NULL);
	
	// ���⼭ ��θ� ������������� ���� ó���� �ʿ�

	// �����ߴٰ� ġ��
	WCHAR* fullPath = (WCHAR*)(tempMemory.GetPointer());
	
	// ����� �ε���
	MINT32 index = 0;

	// ���� ���ڿ��� ���ö����� ����
	while (L'\0' != fullPath[index])
	{
		// ���͸� �������ΰ��
		while (L'\\' == fullPath[index])
		{
			// üũ�� ���ؼ� �ش� ��ġ�� ���Ṯ�ڷ� ����
			fullPath[index] = L'\0';
			
			// �ش� ��ο� ���͸� ����
			if (MFALSE == MakeDirInternal_WINDOWS(fullPath)) {
				return MFALSE;
			}

			// ����
			fullPath[index] = L'\\';
		}
	}

	return MTRUE;
}



MBOOL MFileUtil::MakeDirInternal_WINDOWS(const MWCHAR* inPath)
{
	// ��� ���͸��� �ִ��� üũ�ϰ� ���ٸ� ����

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