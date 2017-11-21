#pragma once
#include <stdint.h>

enum MP_SERVER_TYPES : int8_t
{
	MP_ST_NONE		= -1,    // NONE
	MP_CLIENT		= 0,
	MP_ST_CENTER	= 1,
	MP_ST_SUPER		= 2,    //
	MP_ST_GATE		= 3,    //
	MP_ST_GAME		= 4,
	MP_ST_MAX,

};

static const char * ServerTypeNames[] = 
{
	"Client",
	"CenterServer",
	"SuperServer",
	"GatewayServer",
	"GameServer"
};

#define MAKELONGLONG(a, b) static_cast<long long>(static_cast<unsigned int>(a&0xFFFFFFFF) | static_cast<long long>(b&0xFFFFFFFF) << 32 )