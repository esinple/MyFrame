#include "SuperServerManager.h"
#include "MPDefine.pb.h"

CPP_GAME_MANAGER_REG(SuperServerManager)

SuperServerManager::SuperServerManager()
	: ManagerModule(eGameMgr_SuperServer)
{
}

SuperServerManager::~SuperServerManager()
{
}
bool SuperServerManager::Awake()
{
	REGISTER_SUPER(this, MPMsg::eGate2Super_VerifyMe, SuperServerManager::OnSuperVerify);

	return true;
}
bool SuperServerManager::AfterAwake()
{
	return true;
}
bool SuperServerManager::Execute()
{
	checkVerifySuperServer();
	return true;
}
bool SuperServerManager::BeforeShutDown()
{
	return true;
}
bool SuperServerManager::ShutDown()
{
	return true;
}

void SuperServerManager::AddSuperServer(uint64_t nSockIndex, const char* ip, int nPort)
{
	auto pSuperServer = m_SuperServerPool.Get(nSockIndex, ip, nPort);
	if (pSuperServer == nullptr)
	{
		MP_ERROR("Can not create SuperServer![%lld][%s][%d]", nSockIndex, ip, nPort);
		return;
	}
	if (!m_mTempSuperServers.emplace(nSockIndex, pSuperServer).second)
	{
		MP_ERROR("Same SuperServer![%lld][%s][%d]", nSockIndex, ip, nPort);
		return;
	}
	MP_INFO("New Temp SuperServer![%lld][%s][%d]", nSockIndex, ip, nPort);
}

void SuperServerManager::DelSuperServer(uint64_t nSockIndex)
{
	m_mSuperServers.erase(nSockIndex);
	m_mTempSuperServers.erase(nSockIndex);
}

void SuperServerManager::OnSuperVerify(const std::string& data,const uint64_t nSockIndex)
{
	auto it = m_mTempSuperServers.find(nSockIndex);
	if (it == m_mTempSuperServers.end())
	{
		return;
	}
	auto pSuper = it->second;
	m_mTempSuperServers.erase(it);
	m_mSuperServers.emplace(nSockIndex, pSuper);
}

void SuperServerManager::checkVerifySuperServer()const
{
	for (auto& ssi : m_mTempSuperServers)
	{
		auto pSuperServer = ssi.second;
		pSuperServer->VerifySuperServer();
	}
}