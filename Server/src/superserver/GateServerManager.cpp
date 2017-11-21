#include "GateServerManager.h"
#include "MPDefine.pb.h"
#include "MPDefine.pb.h"
#include "Gate2Super.pb.h"
#include "MPLogger.h"

CPP_SUPER_MANAGER_REG(GateServerManager);

GateServerManager::GateServerManager()
	: ManagerModule(eSuperMgr_GateServer)
{
}

GateServerManager::~GateServerManager()
{
}
bool GateServerManager::Awake()
{
	REGISTER_GATE(this, MPMsg::eGate2Super_VerifyMe, GateServerManager::OnGateVerify);
	return true;
}

bool GateServerManager::AfterAwake()
{
	return true;
}

bool GateServerManager::Execute()
{
	return true;
}

bool GateServerManager::BeforeShutDown()
{
	return true;
}

bool GateServerManager::ShutDown()
{
	return true;
}

void GateServerManager::OnGateVerify(const std::string& sData,const uint64_t nSockIndex)
{
	MPMsg::GateVerify_Super2Gate msg;
	SEND2GATE(nSockIndex,MPMsg::eGate2Super_VerifyMe, msg);
}

void GateServerManager::AddGateServer(uint64_t nSockIndex, const char* ip, int nPort)
{
	auto pGateServer = std::make_shared<GateServer>(nSockIndex, ip, nPort);
	if (pGateServer == nullptr)
	{
		return;
	}
	
	if (!m_mGateServers.emplace(nSockIndex, pGateServer).second)
	{
		MP_ERROR("Same Gate Server!");
	}
}

void GateServerManager::DelGateServer(uint64_t nSockIndex)
{
	m_mGateServers.erase(nSockIndex);
}