#pragma once
#include "SuperManagerModuleEnum.h"

#define REG_MODULE_NAME(e,c) case e:return c;

static const char * GetSuperMgrModuleName(uint32_t type)
{
	switch (type)
	{
		REG_MODULE_NAME(eSuperMgr_GateServer, "GateServerManager");
		REG_MODULE_NAME(eSuperMgr_CenterServer, "CenterServerManager");
		REG_MODULE_NAME(eSuperMgr_GameServer, "GameServerManager");
		REG_MODULE_NAME(eSuperMgr_Time, "SuperTimeManager");
	default:
		return nullptr;
	}
};