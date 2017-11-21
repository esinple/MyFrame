#pragma once
#include <stdint.h>

enum Enum_GatewayManagerModule : int32_t
{
	eGatewayMgr_Unknown		= -1,
	eGatewayMgr_Begin		= 0,
	eGatewayMgr_Time		= 0,
	eGatewayMgr_GateUser	= 1,
	eGatewayMgr_GameServer	= 2,
	eGatewayMgr_SuperServer	= 3,

	eGatewayMgr_End
};