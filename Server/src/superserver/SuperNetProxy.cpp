#include "SuperNetProxy.h"
#include "SuperManagerModuleEnum.h"
#include "SuperManagerModuleDefine.h"
#include "CommDef.h"
#include "GateServerManager.h"
#include "GameServerManager.h"

MP_SINGLETON_IMPLEMENT(SuperNetProxy);

SuperNetProxy::SuperNetProxy() 
	: NetProxy(100),m_Mgrs(eSuperMgr_End,GetSuperMgrModuleName)
{
}

SuperNetProxy::~SuperNetProxy()
{
}

bool SuperNetProxy::InitServerCfg(const std::string& filename)
{
	AddTCPServerModule(MP_ST_GAME, 3000, "127.0.0.1", 16123);
	AddTCPServerModule(MP_ST_GATE, 3000, "127.0.0.1", 16124);

	//AddReceiveCallBack();
	//AddTCPServerModule(MP_ST_CENTER, 3000, 16125);
	return true;
}

void SuperNetProxy::LogicStart()
{
	m_Mgrs.Awake();
}
void SuperNetProxy::LogicFinal()
{
	m_Mgrs.ShutDown();
}

void SuperNetProxy::LogicRun()
{
	m_Mgrs.Execute();
	if (0)
	{
		Kick(1,123);
	}
}

void SuperNetProxy::OnClientDisconnect(const uint8_t nType, const MPSOCK nSockIndex)
{
	switch (nType)
	{
	case MP_ST_GAME:
	{
		auto pGameServerMgr = GetModule<GameServerManager>(eSuperMgr_GameServer);
		pGameServerMgr->DelGameServer(nSockIndex);
		MP_INFO("GameServer Disconnected![%lld]", nSockIndex);
	}
	break;
	case MP_ST_GATE:
	{
		auto pGateServerMgr = GetModule<GateServerManager>(eSuperMgr_GateServer);
		pGateServerMgr->DelGateServer(nSockIndex);
		MP_INFO("GateServer Disconnected![%lld]",nSockIndex);
	}
	break;
	case MP_ST_CENTER:
	{
	}
	break;
	default:
		break;
	}
}

void SuperNetProxy::OnClientConnected(const uint8_t nType, const MPSOCK nSockIndex)
{
	auto pNetModule = GetNetModule(nType);
	auto pNetObject = pNetModule->GetNetObject(nSockIndex);
	if (pNetObject == nullptr)
	{
		return;
	}

	switch (nType)
	{
	case MP_ST_GAME:
	{
		auto pGameServerMgr = GetModule<GameServerManager>(eSuperMgr_GameServer);
		pGameServerMgr->AddGameServer(nSockIndex, pNetObject->GetIP().c_str(), pNetObject->GetPort());
	}
	break;
	case MP_ST_GATE:
	{
		auto pGateServerMgr = GetModule<GateServerManager>(eSuperMgr_GateServer);
		pGateServerMgr->AddGateServer(nSockIndex, pNetObject->GetIP().c_str(), pNetObject->GetPort());
	}
	break;
	case MP_ST_CENTER:
	{
	}
	break;
	default:
		break;
	}
}

const MPGUID SuperNetProxy::GetGUIDBySock(const uint8_t nType, const MPSOCK nSockIndex)const
{
	return 0;
}