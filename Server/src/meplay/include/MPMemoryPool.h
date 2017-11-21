#pragma once

#include <list>
#include <memory>
#include <functional>
#include <stdint.h>

template<typename T>
class MPMemoryPool final
{
public:
	MPMemoryPool(){};
	~MPMemoryPool()
	{
		for (auto& p : m_objMemoryPool)
		{
			if (p != nullptr)
			{
				delete p;
				p = nullptr;
			}
		}
		m_objMemoryPool.clear();
	};
public:
	template<typename... Args>
	std::shared_ptr<T> Get(Args... args)
	{
		std::shared_ptr<T> p(create<Args...>(static_cast<Args&>(args)...),[&](T* pb)->void{ DelMe(pb); });
		return p;
	}

	void DelMe(T* pb)
	{
		m_objMemoryPool.emplace_back(pb);
	}
private:
	template<typename... Args>
	T* create(Args... args)
	{
		T* pRet(nullptr);
		if (m_objMemoryPool.empty())
		{
			//pRet = new T(static_cast<Args&>(std::decay<Args>::type(args)) ...);
			pRet = new T(static_cast<Args&>(args)...);
		}
		else
		{
			pRet = m_objMemoryPool.back();
			m_objMemoryPool.pop_back();
			pRet->ReuseInit(static_cast<Args&>(args)...);
		}
		((T*)pRet)->Clear();
		return pRet;
	}
private:
	std::list<T*> m_objMemoryPool;
};
