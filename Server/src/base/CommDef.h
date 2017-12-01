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

#define U32_MASK 0xFFFFFFFF
#define MAKELONGLONG(a, b) static_cast<uint64_t>((((static_cast<uint64_t>(a))&U32_MASK)<<32) | static_cast<uint64_t>(b&U32_MASK))
#define SPLITLONGLONG(a,b,x) a = x >> 32; b = x & U32_MASK;