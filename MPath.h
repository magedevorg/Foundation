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
		// 스트링 초기화

		if (0 == MStringUtil::GetLength(inPath)) {
			return;
		}



		const MINT32 INIT_COUNT = MAX_PATH;

		MArray<WCHAR, MMemoryI<INIT_COUNT>> fullPath;
		fullPath.SetCount(INIT_COUNT + 1);

		DWORD count = 0;
		{
			count = ::GetFullPathNameW(inPath, INIT_COUNT, &(fullPath[0]), NULL);

			// 메모리가 부족한경우 재할당 해서 다시한번 처리
			if (INIT_COUNT < count)
			{
				fullPath.SetCount(count);

				// 다시 처리
				count = ::GetFullPathNameW(inPath, count, &(fullPath[0]), NULL);
			}

			if (0 == count)
			{
				// 실패
				return;
			}
		}


		MINT32 endDirIndex = -1;
		MINT32 extIndex = -1;

		// 풀패스를 돌면서 경로 인덱스 정보를 만든다
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

		// 무효한 정보
		if (-1 == endDirIndex && -1 == extIndex) {
			return;
		}

		// 확장자를 찾지 못했다면 끝까지 경로로 사용
		if (-1 == extIndex) {
			endDirIndex = count - 1;
		}

		// 디렉터리 종료 인덱스가 있는경우 정보를 복사
		if (-1 != endDirIndex)
		{
			Directory.CopyToCount(&(fullPath[0]), endDirIndex + 1);

			if (L'\\' != Directory[endDirIndex])
			{
				Directory += L"\\";
			}


		}

		// 확장자가 존재하는경우 디렉터리 종료인덱스에서 마지막까지 복사
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
	// 경로 정보
	MString FullPath;
	MString Directory;
	MString FileName;
};

