#pragma once
#include "GameCommDef.h"
#include "UserModuleDefine.h"
#include "MPModule.h"
#include "ModuleRely.h"

class UserModule : public meplay::MPModule,public ModuleRely
{
public:
	UserModule(int32_t nModule) : MPModule(nModule),ModuleRely() {};
	virtual ~UserModule() {};
public:
	virtual bool UserAwake() = 0;
	virtual bool UserAfterAwake() = 0;
	virtual bool UserExecute() = 0;
	virtual bool UserBeforeShutDown() = 0;
	virtual bool UserShutDown() = 0;
private:
};