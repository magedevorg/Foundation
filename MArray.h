#pragma once

#include "MMemory.h"


template<typename T, typename MEMORY_T = MMemory>
class MArray
{
public:
	MArray()
	{

	}

public:
	void Reserve(MINT32 inCount)
	{
		Memory.Alloc(inCount);
	}

	void SetCount(MINT32 inCount)
	{
		Count = inCount;
		Memory.Alloc(inCount, MTRUE);
	}

	MINT32 GetCount() const {
		return Count;
	}

	T& operator[] (MINT32 inIndex) const {
		return ((T*)Memory.GetPointer())[inIndex];
	}

	void Clear()
	{
		Count = 0;
		Memory.Clear();
	}

protected:
	MINT32 Count = 0;

	// ¸Þ¸ð¸®
	MEMORY_T Memory;
};

