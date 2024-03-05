#pragma once

#include "MPrerequisites.h"

#include <stack>
#include <mutex>

#include <functional>




template <typename T>
class MPool
{
public:
	MPool() {}
	virtual ~MPool()
	{
		MSAFE_DELETE(Mutex_Pool);
		
		while (MFALSE == Pool.empty())
		{
			T* obj = Pool.top();
			delete obj;
			Pool.pop();
		}
	}

public:
	void InitPool(MBOOL inThreadSafe, MSIZE inInitCount, MBOOL inIsAutoAdd, std::function<T*()> inCreateLogic)
	{	
		// 멀티 쓰레드에서 사용된다면 뮤택스 객체 생성
		if (MTRUE == inThreadSafe) {
			Mutex_Pool = new std::mutex();
		}

		// 생성 로직 설정
		CreateLogic = inCreateLogic;

		// 풀에 등록
		for (MINDEX i = 0; i < inInitCount; ++i) {
			Pool.push(CreateLogic());
		}

		// 자동 추가 플래그
		IsAutoAdd = inIsAutoAdd;
	}

	void Push(T* inObject)
	{
		if (nullptr != Mutex_Pool)
		{
			std::lock_guard<std::mutex> guard(*Mutex_Pool);
			Pool.push(inObject);
			return;
		}

		Pool.push(inObject);
	}

	T* Pop() 
	{
		if (nullptr != Mutex_Pool)
		{
			std::lock_guard<std::mutex> guard(*Mutex_Pool);
			return PopLogic();
		}
		
		return PopLogic();
	}

protected:
	T* PopLogic()
	{
		if (true == Pool.empty()) 
		{
			if (MTRUE == IsAutoAdd) {
				return CreateLogic();
			}

			return nullptr;
		}

		T* obj = Pool.top();
		Pool.pop();
		return obj;
	}

protected:
	// 뮤텍스
	std::mutex* Mutex_Pool = nullptr;
	std::stack<T*> Pool;

	// 객체 생성 로직
	std::function<T*()> CreateLogic;

	// 모자랄경우 자동으로 추가할것인지
	MBOOL IsAutoAdd = MFALSE;
};




template <typename T>
class MPoolObject
{
public:
	MPoolObject(class MPool<T>& inPool)
		: Pool(inPool)
	{
		ManagedObject = inPool.Pop();
	}

	~MPoolObject()
	{
		// 객체 파괴시 반납
		Pool.Push(ManagedObject);
	}

public:
	T* Get(){
		return ManagedObject;
	}

	T* operator->() {
		return ManagedObject;
	}

protected:
	class MPool<T>& Pool = nullptr;
	T* ManagedObject = nullptr;
};
