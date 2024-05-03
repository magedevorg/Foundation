#pragma once

#include "MString.h"
#include "MArray.h"



class MPath
{
public:
	MPath(const MString& inPath)
	{
		SetPath(inPath.GetStr());
	}

	MPath(const MWCHAR* inPath)
	{
		SetPath(inPath);
	}

public:
	void SetPath(const MWCHAR* inPath)
	{
		// ��Ʈ�� �ʱ�ȭ

		if (0 == MStringUtil::GetLength(inPath)) {
			return;
		}



		const MINT32 INIT_COUNT = MAX_PATH;

		MArray<WCHAR, MMemoryI<INIT_COUNT>> fullPath;
		fullPath.SetCount(INIT_COUNT + 1);

		DWORD count = 0;
		{
			count = ::GetFullPathNameW(inPath, INIT_COUNT, &(fullPath[0]), NULL);

			// �޸𸮰� �����Ѱ�� ���Ҵ� �ؼ� �ٽ��ѹ� ó��
			if (INIT_COUNT < count)
			{
				fullPath.SetCount(count);

				// �ٽ� ó��
				count = ::GetFullPathNameW(inPath, count, &(fullPath[0]), NULL);
			}

			if (0 == count)
			{
				// ����
				return;
			}
		}


		MINT32 endDirIndex = -1;
		MINT32 extIndex = -1;

		// Ǯ�н��� ���鼭 ��� �ε��� ������ �����
		for (DWORD i = 0; i < count; ++i)
		{
			if ('\\' == fullPath[i])
			{
				endDirIndex = i;
			}
			else if ('.' == fullPath[i])
			{
				extIndex = i;
			}
		}

		// ��ȿ�� ����
		if (-1 == endDirIndex && -1 == extIndex) {
			return;
		}

		// Ȯ���ڸ� ã�� ���ߴٸ� ������ ��η� ���
		if (-1 == extIndex) {
			endDirIndex = count - 1;
		}

		// ���͸� ���� �ε����� �ִ°�� ������ ����
		if (-1 != endDirIndex)
		{
			Directory.CopyToCount(&(fullPath[0]), endDirIndex + 1);

			if (L'\\' != Directory[endDirIndex])
			{
				Directory += L"\\";
			}


		}

		// Ȯ���ڰ� �����ϴ°�� ���͸� �����ε������� ���������� ����
		if (-1 != extIndex)
		{
			FileName.CopyToCount(&(fullPath[endDirIndex + 1]), count - (endDirIndex + 1));
		}
	}

	const MString& GetDirectory() const {
		return Directory;
	}

	const MString& GetFileName() const {
		return FileName;
	}

protected:
	// ��� ����
	MString FullPath;
	MString Directory;
	MString FileName;
};

