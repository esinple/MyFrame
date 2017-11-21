#include "StatsServerManager.h"
#include "MPDefine.pb.h"

CPP_GAME_MANAGER_REG(StatsServerManager)

StatsServerManager::StatsServerManager()
	: ManagerModule(eGameMgr_StatsServer)
{
}

StatsServerManager::~StatsServerManager()
{
}
bool StatsServerManager::Awake()
{
	//REGISTER_SUPER(this, MPMsg::eGate2Stats_VerifyMe, StatsServerManager::OnStatsVerify);

	return true;
}
bool StatsServerManager::AfterAwake()
{
	return true;
}
bool StatsServerManager::Execute()
{
	return true;
}
bool StatsServerManager::BeforeShutDown()
{
	return true;
}
bool StatsServerManager::ShutDown()
{
	return true;
}

void StatsServerManager::AddStatsServer(uint64_t nSockIndex, const char* ip, int nPort)
{
	auto pStatsServer = m_StatsServerPool.Get(nSockIndex, ip, nPort);
	if (pStatsServer == nullptr)
	{
		MP_ERROR("Can not create StatsServer![%lld][%s][%d]", nSockIndex, ip, nPort);
		return;
	}
	if (!m_mTempStatsServers.emplace(nSockIndex, pStatsServer).second)
	{
		MP_ERROR("Same StatsServer![%lld][%s][%d]", nSockIndex, ip, nPort);
		return;
	}
	MP_INFO("New Temp StatsServer![%lld][%s][%d]", nSockIndex, ip, nPort);
}

void StatsServerManager::DelStatsServer(uint64_t nSockIndex)
{
	m_mStatsServers.erase(nSockIndex);
	m_mTempStatsServers.erase(nSockIndex);
}