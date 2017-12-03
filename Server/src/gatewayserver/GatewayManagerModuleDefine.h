#pragma once
#include <stdint.h>

enum Enum_GatewayManagerModule : int32_t
{
	eGateMgr_Unknown = -1,
	eGateMgr_Begin = 0,
	eGateMgr_Time = 0,
	eGateMgr_GateUser = 1,
	eGateMgr_GameServer = 2,
	eGateMgr_SuperServer = 3,

	eGateMgr_End
};