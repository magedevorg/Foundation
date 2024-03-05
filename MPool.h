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
		// ��Ƽ �����忡�� ���ȴٸ� ���ý� ��ü ����
		if (MTRUE == inThreadSafe) {
			Mutex_Pool = new std::mutex();
		}

		// ���� ���� ����
		CreateLogic = inCreateLogic;

		// Ǯ�� ���
		for (MINDEX i = 0; i < inInitCount; ++i) {
			Pool.push(CreateLogic());
		}

		// �ڵ� �߰� �÷���
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
	// ���ؽ�
	std::mutex* Mutex_Pool = nullptr;
	std::stack<T*> Pool;

	// ��ü ���� ����
	std::function<T*()> CreateLogic;

	// ���ڶ���� �ڵ����� �߰��Ұ�����
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
		// ��ü �ı��� �ݳ�
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
