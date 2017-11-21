#include "CenterServerManager.h"

CPP_SUPER_MANAGER_REG(CenterServerManager);

CenterServerManager::CenterServerManager()
	: ManagerModule(eSuperMgr_CenterServer)
{
}

CenterServerManager::~CenterServerManager()
{
}
bool CenterServerManager::Awake()
{
	return true;
}

bool CenterServerManager::AfterAwake()
{
	return true;
}

bool CenterServerManager::Execute()
{
	return true;
}

bool CenterServerManager::BeforeShutDown()
{
	return true;
}

bool CenterServerManager::ShutDown()
{
	return true;
}