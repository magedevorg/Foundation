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

};

