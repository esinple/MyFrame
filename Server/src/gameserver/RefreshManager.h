#pragma once
#include "GameCommDef.h"

class RefreshManager : public ManagerModule
{
public:
	RefreshManager();
	~RefreshManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
private:
};

GAME_MANAGER_MODULE_REG(eGameMgr_Refresh, RefreshManager);