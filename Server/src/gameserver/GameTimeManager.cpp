#include "GameTimeManager.h"

//CPP_GAME_MANAGER_REG(GameTimeManager);

GameTimeManager::GameTimeManager()
	: ManagerModule(eGameMgr_Time)
{
}

GameTimeManager::~GameTimeManager()
{
}
bool GameTimeManager::Awake()
{
	return true;
}

bool GameTimeManager::AfterAwake()
{
	return true;
}

bool GameTimeManager::Execute()
{
	m_CurTime.Now();

	m_TimeMgr.Run();
	return true;
}

bool GameTimeManager::BeforeShutDown()
{
	return true;
}

bool GameTimeManager::ShutDown()
{
	return true;
}