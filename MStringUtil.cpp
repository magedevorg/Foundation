#include "MStringUtil.h"
#include <string.h>
#include "MPlatformBase.h"


#if (MSTRING_CONVERT == MSTRING_CONVERT_WINAPI)

std::array<MCHAR_SET_ID, (MSIZE)EMCharset::Max> MStringUtil::CharSetIDList =
{
	CP_UTF8,
	0,			// Ÿ���� ���� ����
};

EMError MStringUtil::Convert(MMemory& outDest, EMCharset inDestCharset, const void* inSrc, EMCharset inSrcCharset)
{
	// multibyte -> wchar
	auto WCharConvFunc = [](MMemory& outDest, MCHAR_SET_ID inCharset, const void* inStr)
	{
		const char* str = (const char*)inStr;

		const MSIZE strLength = ::strlen(str);
		const int wcharLength = ::MultiByteToWideChar(inCharset, 0, str, strLength, nullptr, 0);
		const int wcharCapacity = (wcharLength + 1) * sizeof(wchar_t);

		// ���� �Ҵ�
		outDest.Alloc(wcharCapacity, MTRUE);
		outDest.Clear();

		::MultiByteToWideChar(inCharset, 0, str, strLength, (MWCHAR*)outDest.GetPointer(), wcharLength);
	};

	// wchar -> multibyte
	auto CharConvFunc = [](MMemory& outDest, MCHAR_SET_ID inCharset, const void* inStr)
	{
		const wchar_t* str = (const wchar_t*)inStr;

		const MSIZE srcLength = ::wcslen(str);
		const int charLength = ::WideCharToMultiByte(inCharset, 0, str, srcLength, nullptr, 0, nullptr, nullptr);
		const int charCapacity = (charLength + 1) * sizeof(char);

		// ���� �Ҵ�
		outDest.Alloc(charCapacity, MTRUE);
		outDest.Clear();

		::WideCharToMultiByte(inCharset, 0, str, srcLength, (MCHAR*)outDest.GetPointer(), charLength, nullptr, nullptr);
	};

	MCHAR_SET_ID srcID = GetIDByCharset(inSrcCharset);
	MCHAR_SET_ID destID = GetIDByCharset(inDestCharset);


	if (EMCharset::WCHAR == inSrcCharset)
	{
		// ��Ƽ����Ʈ�� ��ȯ
		if (EMCharset::WCHAR == inDestCharset)
		{
			// �̷��� ����ϸ� �ȵ�
		}
		else
		{
			CharConvFunc(outDest, destID, inSrc);
		}
	}
	else
	{
		// �ҽ��� ��Ƽ����Ʈ�ΰ��

		const MSIZE ADD_SIZE = 512;
		MMemoryI<ADD_SIZE> tempMemory;

		if (EMCharset::WCHAR == inDestCharset)
		{
			// ����� wchar��� �ٷ� dest�� ���� �ϰ� ����
			WCharConvFunc(outDest, srcID, inSrc);
			return EMError::None;
		}
		else
		{
			// �ƴ϶�� �ӽ� �޸𸮿� ����
			WCharConvFunc(tempMemory, srcID, inSrc);
		}

		// ��Ƽ����Ʈ�� ��ȯ
		CharConvFunc(outDest, destID, tempMemory.GetPointer());
	}

	return EMError::None;
}

#else
#	include "iconv.h"
# 	include <cerrno>
std::array<MCHAR_SET_ID, (int)EMCharset::Max> MStringUtil::CharSetIDList =
{
	"utf-8",
	"wchar_t"
};


EMError MStringUtil::Convert(MMemory& outDest, EMCharset inDestCharset, const void* inSrc, EMCharset inSrcCharset)
{
	/*
	assert(nullptr != inSrc);

	MCHAR_SET_ID srcID = GetIDByCharset(inSrcCharset);
	MCHAR_SET_ID destID = GetIDByCharset(inDestCharset);

	// iconv
	iconv_t handle = iconv_open(destID, srcID);
	if ((iconv_t)(-1) == handle) {
		return EMError::MError_Test;
	}

	// �ҽ� ����
	const MSIZE srcSizeBackup = MStringUtil::GetSize(inSrcCharset, inSrc);
	char* srcPtr = nullptr;
	size_t srcSize = 0;

	// �ӽ� �޸� ����
	const MSIZE ADD_SIZE = 512;               // ���� ��
	MMemoryI<ADD_SIZE> tempMemory;

	char* tempPtr = nullptr;
	size_t tempSize = 0;

	// ��� �÷���
	EMError error = EMError::None;

	// ������ ���鼭 ó��
	while (true)
	{
		// �ҽ� ���� 
		srcPtr = (char*)inSrc;
		srcSize = srcSizeBackup;

		// �ӽ� ���� ����
		tempPtr = (char*)tempMemory.GetPointer();
		tempSize = tempMemory.GetSize();

		size_t ret = iconv(handle, &srcPtr, &srcSize, &tempPtr, &tempSize);
		if (-1 != ret) {
			break;      // �������� ó��
		}

		// ������ �߻�
		if (E2BIG == errno)
		{
			// �ƿ� ���ۿ� ������ ������� ����
			// ���۸� ���Ҵ��Ŀ� �ٽ� �õ�
			tempMemory.Alloc(tempMemory.GetSize() + ADD_SIZE);
		}
		else
		{
			// �׿� ������ �����ڵ� ������ ������
			switch (errno)
			{
			case EILSEQ:
			{
				// �Է¿��� �߸��� ��Ƽ����Ʈ ������ �߰�
				error = EMError::MError_Test;
			} break;
			case EINVAL:
			{
				// �Ϸ���� ���� ��Ƽ����Ʈ ���ڿ��� �Է°����ε���
				error = EMError::MError_Test;
			} break;
			}

			break;
		}
	}

	if (EMError::None == error)
	{
		// ����ó�� dest�� ī��

		// ī���� ����� ��´�
		const MSIZE capacity = tempMemory.GetSize() - tempSize;
		if (0 == capacity)
		{
			// ī���� ������ ���ٸ� Ŭ����
			outDest.Clear();
		}
		else
		{
			// ī���� ������ �ִٸ� ���๮�ڻ���� �߰��ؼ� ���� ó��
			outDest.Alloc(capacity);
			outDest.Write(0, tempMemory.GetPointer(), capacity, MTRUE);
		}
	}

	iconv_close(handle);

	return error;
	 */

	return EMError::None;
}


#endif



MSIZE MStringUtil::GetSize(EMCharset inCharset, const void* inStr)
{
	if (nullptr == inStr) {
		return 0;
	}

	MSIZE capacity = 0;
	switch (inCharset)
	{
	case EMCharset::WCHAR:
	{
		// ���ϵ� �ɸ���
		capacity = wcslen((const MWCHAR*)inStr) * sizeof(MWCHAR);
		capacity += sizeof(MWCHAR);
	} break;
	default:
	{
		// �켱 �׿�
		capacity = ::strlen((const MCHAR*)inStr);
		capacity += sizeof(MCHAR);
		break;
	}
	}

	return capacity;
}



// Ÿ������ �ɸ��� �� ���̵� ��´�
MCHAR_SET_ID MStringUtil::GetIDByCharset(EMCharset inCharset)
{
	const MINT32 index = (MINT32)inCharset - 1;
	return CharSetIDList[index];
}