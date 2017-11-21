#include "SuperTimeManager.h"

CPP_SUPER_MANAGER_REG(SuperTimeManager);

SuperTimeManager::SuperTimeManager()
	: ManagerModule(eSuperMgr_Time)
{
}

SuperTimeManager::~SuperTimeManager()
{
}
bool SuperTimeManager::Awake()
{
	return true;
}

bool SuperTimeManager::AfterAwake()
{
	return true;
}

bool SuperTimeManager::Execute()
{
	m_CurTime.Now();
	m_TimeMgr.Run();
	return true;
}

bool SuperTimeManager::BeforeShutDown()
{
	return true;
}

bool SuperTimeManager::ShutDown()
{
	return true;
}