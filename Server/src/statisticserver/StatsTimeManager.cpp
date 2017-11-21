#include "StatsTimeManager.h"

CPP_STATS_MANAGER_REG(StatsTimeManager);

StatsTimeManager::StatsTimeManager()
	: ManagerModule(eStatsMgr_Time)
{
}

StatsTimeManager::~StatsTimeManager()
{
}
bool StatsTimeManager::Awake()
{
	return true;
}

bool StatsTimeManager::AfterAwake()
{
	return true;
}

bool StatsTimeManager::Execute()
{
	m_CurTime.Now();
	return true;
}

bool StatsTimeManager::BeforeShutDown()
{
	return true;
}

bool StatsTimeManager::ShutDown()
{
	return true;
}