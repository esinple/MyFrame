#include "GameManagerModuleDefine.h"

const char * GetGameMgrModuleName(uint32_t type)
{
	switch (type)
	{
		REG_MODULE_NAME(eGameMgr_Time, "GameTimeManager");
		REG_MODULE_NAME(eGameMgr_SuperServer, "SuperServerManager");
		REG_MODULE_NAME(eGameMgr_GateServer, "GateServerManager");
		REG_MODULE_NAME(eGameMgr_StatsServer, "StatsServerManager");
		REG_MODULE_NAME(eGameMgr_GameUser, "GameUserManager");
		REG_MODULE_NAME(eGameMgr_DBServer, "DBManager");
	default:
		return nullptr;
	}
};