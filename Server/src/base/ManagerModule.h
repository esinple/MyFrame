#pragma once
#include "MPModule.h"
#include "MPLogger.h"
#include "MPMemoryPool.h"
#include "MPTime.h"
#include "MPTimeTester.h"
#include "ModuleRely.h"

class ManagerModule : public meplay::MPModule,public ModuleRely
{
public:
	ManagerModule(uint32_t nModuleType);
	virtual ~ManagerModule();
public:
	virtual bool Awake() = 0;
	virtual bool AfterAwake() = 0;
	virtual bool Execute() = 0;
	virtual bool BeforeShutDown() = 0;
	virtual bool ShutDown() = 0;
};

#define _1_SEC_TICK(LAST_TICK,NOW) LAST_TICK + 1 < NOW