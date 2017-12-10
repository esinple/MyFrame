#pragma once
#include "MPGUID.h"

typedef int64_t MPSOCK;

enum MP_NET_TYPE : uint8_t
{
	MP_NET_TYPE_UNKNOWN = 0,
	MP_NET_TYPE_TCP_SERVER = 1,
	MP_NET_TYPE_TCP_CLIENT = 2,
	MP_NET_TYPE_UDP_SERVER = 3,
};