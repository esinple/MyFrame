#include "GateTimeManager.h"

CPP_GATE_MANAGER_REG(GateTimeManager);

GateTimeManager::GateTimeManager() 
	: ManagerModule(eGateMgr_Time)
{
}

GateTimeManager::~GateTimeManager()
{
}
bool GateTimeManager::Awake()
{

	return true;
}

bool GateTimeManager::AfterAwake()
{
	return true;
}

bool GateTimeManager::Execute()
{
	m_CurTime.Now();
	return true;
}

bool GateTimeManager::BeforeShutDown()
{
	return true;
}

bool GateTimeManager::ShutDown()
{
	return true;
}