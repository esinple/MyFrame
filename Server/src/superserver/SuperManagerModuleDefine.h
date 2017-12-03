#pragma once
#include <stdint.h>

enum Enum_SuperManagerModule : int32_t
{
	eSuperMgr_Unknown = -1,
	eSuperMgr_Begin = 0,
	eSuperMgr_GateServer = 0,
	eSuperMgr_CenterServer = 1,
	eSuperMgr_GameServer = 2,
	eSuperMgr_Time = 3,

	eSuperMgr_End
};