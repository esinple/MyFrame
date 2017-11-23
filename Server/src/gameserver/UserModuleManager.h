#pragma once

#pragma once
#include <vector>
#include "UserModule.h"

typedef const char*(*GetUserModuleFuncPtr)(uint32_t);

class UserModuleManager final
{
public:
	UserModuleManager(uint32_t nModuleNum,GetUserModuleFuncPtr pFunc);
	~UserModuleManager();
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
	std::vector<UserModule*>	m_vModules;
	uint32_t					m_nModuleNum;
	GetUserModuleFuncPtr		m_pFunc;
	static std::vector<int>		m_vOrder;
};