#pragma once
#include <vector>
#include "ManagerModule.h"

typedef const char*(*GetNameFuncPtr)(uint32_t);

class ManagerModuleManager final
{
public:
	ManagerModuleManager(uint32_t nModuleNum, GetNameFuncPtr pFunc);
	~ManagerModuleManager();
public:
	bool Awake();
	bool Execute();
	bool ShutDown();

	//modules
	template<typename T>
	T* GetModule(uint32_t nType)
	{
		if (nType >= m_nModuleNum)
			return nullptr;
		return (T*)m_vModules[nType];
	}

	template<typename T>
	const T* GetModule(uint32_t nType)const
	{
		if (nType >= m_nModuleNum)
			return nullptr;
		return (T*)m_vModules[nType];
	}
private:
	bool topologicalSort();
private:
	std::vector<ManagerModule*> m_vModules;
	uint32_t					m_nModuleNum;
	GetNameFuncPtr				m_pFunc;
	std::vector<int>			m_vOrder;
};