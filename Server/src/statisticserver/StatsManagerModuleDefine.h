#pragma once
#include <stdint.h>

enum Enum_StatsManagerModule : int32_t
{
	eStatsMgr_Unknown = -1,
	eStatsMgr_Begin = 0,
	eStatsMgr_Time = 0,
	eStatsMgr_RocketMQ = 1,

	eStatsMgr_End
};
