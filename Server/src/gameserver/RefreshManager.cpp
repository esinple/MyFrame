#include "RefreshManager.h"

CPP_GAME_MANAGER_REG(RefreshManager)

RefreshManager::RefreshManager() : ManagerModule(eGameMgr_Refresh)
{
	AddParent(eGameMgr_TimeStamp);
}

RefreshManager::~RefreshManager()
{
}

bool RefreshManager::Awake()
{
	return true;
}

bool RefreshManager::AfterAwake()
{
	return true;
}

bool RefreshManager::Execute()
{
	return true;
}

bool RefreshManager::BeforeShutDown()
{
	return true;
}

bool RefreshManager::ShutDown()
{
	return true;
}