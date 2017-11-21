#include "SuperServerManager.h"
#include "MPDefine.pb.h"

CPP_GATE_MANAGER_REG(SuperServerManager)

SuperServerManager::SuperServerManager()
	: ManagerModule(eGatewayMgr_SuperServer)
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
	mtx.lock();
	if (!m_mTempSuperServers.emplace(nSockIndex, pSuperServer).second)
	{
		mtx.unlock();
		MP_ERROR("Same SuperServer![%lld][%s][%d]", nSockIndex, ip, nPort);
		return;
	}
	mtx.unlock();
	MP_INFO("New Temp SuperServer![%lld][%s][%d]", nSockIndex, ip, nPort);
}

void SuperServerManager::DelSuperServer(uint64_t nSockIndex)
{
	m_mSuperServers.erase(nSockIndex);
	mtx.lock();
	m_mTempSuperServers.erase(nSockIndex);
	mtx.unlock();
}

void SuperServerManager::OnSuperVerify(const std::string& data,const uint64_t nSockIndex)
{
	auto it = m_mTempSuperServers.find(nSockIndex);
	if (it == m_mTempSuperServers.end())
	{
		return;
	}
	auto pSuper = it->second;
	mtx.lock();
	m_mTempSuperServers.erase(it);
	mtx.unlock();
	m_mSuperServers.emplace(nSockIndex, pSuper);
}

void SuperServerManager::checkVerifySuperServer()
{
	mtx.lock();
	for (auto& ssi : m_mTempSuperServers)
	{
		auto pSuperServer = ssi.second;
		pSuperServer->VerifySuperServer();
	}
	mtx.unlock();
}