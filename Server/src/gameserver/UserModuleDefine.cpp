#include "UserModuleDefine.h"

const char * GetUserModuleNameFunc(uint32_t type)
{
	switch (type)
	{
		REG_USER_MODULE_NAME(eGameUser_Base, "UserBase");
	default:
		return nullptr;
	}
};