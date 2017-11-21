#pragma once
#include "StatsManagerModuleEnum.h"

#define REG_MODULE_NAME(e,c) case e:return c;

static const char * GetStatsMgrModuleName(uint32_t type)
{
	switch (type)
	{
		REG_MODULE_NAME(eStatsMgr_Time, "StatsTimeManager");
		REG_MODULE_NAME(eStatsMgr_RocketMQ, "RocketMQManager");
	default:
		return nullptr;
	}
};
