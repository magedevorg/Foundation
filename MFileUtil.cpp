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
	// ���͸��� �и�



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
	// �ʱ� �޸� ������
	const MINT32 INIT_COUNT = 3;

	// ��� ������ splitpath���

	// �޸�
	MMemoryI<(INIT_COUNT + 2) * sizeof(WCHAR)> tempMemory;
	tempMemory.Clear();

	// ��ü ��η� �����
	{
		DWORD count = ::GetFullPathNameW(inPath.GetStr(), INIT_COUNT, (WCHAR*)tempMemory.GetPointer(), NULL);

		// �޸𸮰� �����Ѱ�� ���Ҵ� �ؼ� �ٽ��ѹ� ó��
		if (INIT_COUNT < count)
		{
			tempMemory.Alloc((count + 2) * sizeof(WCHAR));
			tempMemory.Clear();

			// �ٽ� ó��
			count = ::GetFullPathNameW(inPath.GetStr(), count, (WCHAR*)(tempMemory.GetPointer()), NULL);
		}

		if (0 == count)
		{
			// ����
			return MFALSE;
		}

		// ��Ż ��ΰ� ���������� �����Ǿ��ٸ� �������� '\\' ���� üũ�ϰ� �ƴ϶�� ����
		WCHAR lastChar = ((WCHAR*)tempMemory.GetPointer())[count - 1];
		if ('\\' != lastChar)
		{
			((WCHAR*)tempMemory.GetPointer())[count] = '\\';
		}
	}
	
	// �����ߴٰ� ġ��
	WCHAR* fullPath = (WCHAR*)(tempMemory.GetPointer());
	
	// ����� �ε���
	MINT32 index = 0;

	// ���� ���ڿ��� ���ö����� ����
	while (L'\0' != fullPath[index])
	{
		// ���͸� �������ΰ��
		if( L'\\' == fullPath[index] )
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