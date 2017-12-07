#pragma once
#include "ManagerModule.h"
#include "StatsCommDef.h"
#include "MPTime.h"

class StatsTimeManager final : public ManagerModule
{
public:
	StatsTimeManager();
	~StatsTimeManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	inline const meplay::MPTime& CurrentTime()const { return m_CurTime; };
private:
	meplay::MPTime m_CurTime;
};

STATS_MANAGER_MODULE_REG(eStatsMgr_Time, StatsTimeManager);
