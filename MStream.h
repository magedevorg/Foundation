#pragma once


#include "MPrerequisites.h"
#include <memory.h>




//--------------------------------------------------------
// 직렬화 클래스
// 사용하는곳에서 상황에 맞게 stream을 설정
// 직렬화를 원하면 write stream, 역직렬화를 원하면 read stream을 설정
//--------------------------------------------------------
class MSerializable
{
public:
	virtual ~MSerializable() {}

public:
	// 직렬화/역직렬화
	virtual void Serialize(class MStream& inStream) = 0;
};



//--------------------------------------------------------
// 스트림 클래스
// Process라는 하나의 인터페이스로 Read / Write 처리
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
	// 동일한 이름의 2개의 virtual 함수가 있을때
	// 상속받은곳에서 하나의 함수만 재정의시 해당 클래스를 사용할때 재정의한 함수형태만 노출됨
	// 버그?
	virtual void ProcessData(void* inData, MSIZE inSize) = 0;
};



//--------------------------------------------------------
// 바이트 스트림
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
	// 대상 바이드
	void* Bytes = nullptr;

	// 위치 정보
	MINDEX Pos = 0;
};

// 읽기 스트림
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