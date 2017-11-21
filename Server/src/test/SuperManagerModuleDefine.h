#pragma once
#include "SuperManagerModuleEnum.h"

#define REG_MODULE_NAME(e,c) case e:return c;

const char * GetSuperMgrModuleName(uint32_t type)
{
	switch (type)
	{
		REG_MODULE_NAME(eSuperManagerModule_Test, "TMManager");
	default:
		return nullptr;
	}
};