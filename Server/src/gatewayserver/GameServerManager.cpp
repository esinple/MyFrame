#include "GameServerManager.h"

GameServerManager::GameServerManager()
	: ManagerModule(eGateMgr_GameServer)
{
}

GameServerManager::~GameServerManager()
{
}

bool GameServerManager::Awake()
{
	return true;
}
bool GameServerManager::AfterAwake()
{
	return true;
}
bool GameServerManager::Execute()
{
	return true;
}
bool GameServerManager::BeforeShutDown()
{
	return true;
}
bool GameServerManager::ShutDown()
{
	return true;
}

void GameServerManager::AddGameServer(uint64_t nSockIndex,const char* ip, int nPort)
{
	auto pGameServer = m_GameServerPool.Get(nSockIndex, ip, nPort);
	if (pGameServer == nullptr)
	{
		MP_ERROR("Cannot Create GameServer! SockIndex [%lld],ip [%s],port [%d]", nSockIndex, ip, nPort);
		return;
	}
	if (!m_mGameServers.emplace(nSockIndex, pGameServer).second)
	{
		MP_ERROR("Same Sock GameServer! SockIndex [%lld],ip [%s],port [%d]", nSockIndex, ip, nPort);
		return;
	}

	MP_INFO("New GameServer Connected! SockIndex [%lld],ip [%s],port [%d]", nSockIndex, ip, nPort);
	//pGameServer->InitNetInfo();
}

void GameServerManager::DelGameServer(uint64_t nSockIndex)
{
#ifdef _DEBUG 
	auto it = m_mGameServers.find(nSockIndex);
	if (it != m_mGameServers.end())
	{
		MP_DEBUG("Del GameServer [%lld][%s][%d]", nSockIndex, it->second->GetIP().c_str(), it->second->GetPort());
	}
#endif
	m_mGameServers.erase(nSockIndex);
}