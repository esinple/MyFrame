#pragma once
#include "UserModuleDefine.h"
#include "MPModule.h"

class UserModule : public meplay::MPModule
{
public:
	UserModule(int32_t nModule) : MPModule(nModule) {};
	virtual ~UserModule() {};
public:
	virtual bool UserAwake() = 0;
	virtual bool UserAfterAwake() = 0;
	virtual bool UserExecute() = 0;
	virtual bool UserBeforeShutDown() = 0;
	virtual bool UserShutDown() = 0;
private:
};