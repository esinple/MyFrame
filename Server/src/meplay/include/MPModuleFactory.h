#pragma once
#include <map>
#include <stdint.h>
#include <functional>
#include "MPModule.h"

class MPModuleFactory final
{
public:
	template<typename T>
	struct register_t
	{
		register_t(uint32_t nType,uint32_t nIndex, std::string sName)
		{
			MPModuleFactory::GetInstance()->m_mCreateMap[nType].emplace(nIndex, [] { return new T(); });
			MPModuleFactory::GetInstance()->m_mNameMap[nType].emplace(nIndex, sName);
		}
	};

public:
	static MPModuleFactory* GetInstance()
	{
		static MPModuleFactory instance;
		return &instance;
	}
	meplay::MPModule* Create(uint32_t nType, uint32_t nIndex);
	std::string_view GetName(uint32_t nType, uint32_t nIndex)const;
private:
	MPModuleFactory();
	~MPModuleFactory();
	MPModuleFactory(const MPModuleFactory&) = delete; 
	MPModuleFactory(MPModuleFactory&&) = delete; 
private:
	std::map<uint32_t, std::map<uint32_t, std::function<meplay::MPModule*()>>> m_mCreateMap;
	std::map<uint32_t, std::map<uint32_t, std::string>> m_mNameMap;
};

#define AUTO_REGISTER(TYPE,INDEX,NAME) static MPModuleFactory::register_t<NAME> reg_##NAME##_(TYPE,INDEX,std::move(#NAME));