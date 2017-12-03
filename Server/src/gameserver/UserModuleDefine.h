#pragma once
#include "UserModuleEnum.h"
#include "MPAutoRegister.h"
#include "UserModule.h"

#define H_USER_MODULE_REG(INDEX,SUB) H_AUTO_REGISTER_SUB(INDEX,UserModule,SUB)
#define CPP_USER_MODULE_REG(SUB) CPP_AUTO_REGISTER_SUB(SUB)


#define REG_USER_MODULE_NAME(e,c) case e:return c;

extern const char * GetUserModuleNameFunc(uint32_t type);
