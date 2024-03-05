#pragma once


#include "MPrerequisites.h"
#include <memory.h>




//--------------------------------------------------------
// ����ȭ Ŭ����
// ����ϴ°����� ��Ȳ�� �°� stream�� ����
// ����ȭ�� ���ϸ� write stream, ������ȭ�� ���ϸ� read stream�� ����
//--------------------------------------------------------
class MSerializable
{
public:
	virtual ~MSerializable() {}

public:
	// ����ȭ/������ȭ
	virtual void Serialize(class MStream& inStream) = 0;
};



//--------------------------------------------------------
// ��Ʈ�� Ŭ����
// Process��� �ϳ��� �������̽��� Read / Write ó��
//--------------------------------------------------------
class MStream
{
public:
	virtual ~MStream() {}

public:
	virtual MBOOL IsRead() = 0;
	void Process(void* inData, MSIZE inSize)
	{
		ProcessData(inData, inSize);
	}

	void Process(MSerializable& inData)
	{
		inData.Serialize(*this);
	}

protected:
	// ������ �̸��� 2���� virtual �Լ��� ������
	// ��ӹ��������� �ϳ��� �Լ��� �����ǽ� �ش� Ŭ������ ����Ҷ� �������� �Լ����¸� �����
	// ����?
	virtual void ProcessData(void* inData, MSIZE inSize) = 0;
};



//--------------------------------------------------------
// ����Ʈ ��Ʈ��
//--------------------------------------------------------
class MByteStream : public MStream
{
public:
	MByteStream(void* inBytes)
		: Bytes(inBytes)
	{}

public:
	void Reset() {
		Pos = 0;
	}
	
	MINDEX GetPos() const {
		return Pos;
	}

protected:
	// ��� ���̵�
	void* Bytes = nullptr;

	// ��ġ ����
	MINDEX Pos = 0;
};

// �б� ��Ʈ��
class MByteReadStream : public MByteStream
{
public:
	MByteReadStream(void* inBytes)
		: MByteStream(inBytes)
	{}

public:
	virtual MBOOL IsRead() override {
		return MTRUE;
	}

protected:
	virtual void ProcessData(void* inData, MSIZE inSize) override
	{
		::memcpy(inData, (MBYTE*)Bytes + Pos, inSize);
		Pos += inSize;
	}
};

class MByteWriteStream : public MByteStream
{
public:
	MByteWriteStream(MBYTE* inBytes)
		: MByteStream(inBytes)
	{}

public:
	virtual MBOOL IsRead() override {
		return MFALSE;
	}

protected:
	virtual void ProcessData(void* inData, MSIZE inSize) override
	{
		::memcpy((MBYTE*)Bytes + Pos, inData, inSize);
		Pos += inSize;
	}
};