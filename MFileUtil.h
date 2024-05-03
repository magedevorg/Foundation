#pragma once


#include "MPrerequisites.h"
#include "MMemory.h"
#include "MString.h"


class MFileUtil
{
public:
	// ������ �ε�
	static MBOOL LoadFile(MMemory& inMemory, const MString& inPath);

	// ���Ͽ� ����
	static MBOOL SaveToFile(MMemory& inMemory, const MString& inPath);
	static MBOOL SaveToFile(const void* inData, MSIZE inSize, const MString& inPath);


	// ���丮�� ����
	static MBOOL MakeDirectory(const MString& inPath);

private:

#if (MPLATFORM == MPLATFORM_WINDOWS)
	// ���͸��� �ִ��� üũ�ϰ� ���ٸ� ����
	static MBOOL MakeDir_WINDOWS(const MString& inPath);

	// ���͸� ����
	static MBOOL MakeDirInternal_WINDOWS(const MWCHAR* inPath);
#endif

};

