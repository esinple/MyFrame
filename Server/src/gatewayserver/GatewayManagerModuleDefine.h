#ifndef _GATE_MODULE_DEFINE_H_
#define _GATE_MODULE_DEFINE_H_

#include "GatewayManagerModuleEnum.h"

#define REG_MODULE_NAME(e,c) case e:return c;

static const char * GetGatewayMgrModuleName(uint32_t type)
{
	switch (type)
	{
		REG_MODULE_NAME(eGatewayMgr_Time, "GateTimeManager");
		REG_MODULE_NAME(eGatewayMgr_GateUser, "GateUserManager");
		REG_MODULE_NAME(eGatewayMgr_GameServer, "GameServerManager");
		REG_MODULE_NAME(eGatewayMgr_SuperServer, "SuperServerManager");
	default:
		return nullptr;
	}
};
#endif