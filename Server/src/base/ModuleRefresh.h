#pragma once
#include <stdint.h>
#include <any>

class ModuleRefresh
{
public:
	ModuleRefresh();
	virtual ~ModuleRefresh();
public:
	virtual void OnModuleRefresh(uint32_t nMainType,uint32_t nSubType,std::any param) = 0;
};