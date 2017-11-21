#pragma once
#include "ManagerModule.h"
#include "SuperCommDef.h"

class TMManager final : public ManagerModule
{
public:
	TMManager();
	virtual ~TMManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
private:

	H_SUPER_MANAGER_REG(TMManager)
};