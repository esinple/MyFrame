#include "GatewayNetProxy.h"
#include "GatewayManagerModuleDefine.h"
#include "CommDef.h"
#include "GateUser.h"
#include "MPLogger.h"
#include "GateUserManager.h"
#include "GameServerManager.h"
#include "SuperServerManager.h"

MP_SINGLETON_IMPLEMENT(GatewayNetProxy);


GatewayNetProxy::GatewayNetProxy() 
	: NetProxy(100),m_Mgrs(eGateManagerModule,eGateMgr_End)
{
}

GatewayNetProxy::~GatewayNetProxy()
{
}

bool GatewayNetProxy::InitServerCfg(const std::string& filename)
{
	/*AddTCPServerModule(MP_ST_GAME, 3000, 16123);
	AddTCPServerModule(MP_ST_GATE, 3000, 16124);*/

	AddTCPClientModule(MP_ST_GAME, "127.0.0.1", 16125, true);
	AddTCPClientModule(MP_ST_SUPER, "127.0.0.1", 16124, true);
	//AddTCPClientModule(MP_ST_SUPER, "127.0.0.1", 16124, true);

	AddTCPServerModule(MP_CLIENT, 2048, "0.0.0.0",17000);

	//AddReceiveCallBack();
	//AddTCPServerModule(MP_ST_CENTER, 3000, 16125);
	return true;
}

void GatewayNetProxy::LogicStart()
{
	m_Mgrs.Awake();
}
void GatewayNetProxy::LogicFinal()
{
	m_Mgrs.ShutDown();
}

void GatewayNetProxy::LogicRun()
{
	m_Mgrs.Execute();

	if (0)
	{
		this->Final("");
	}

}

void GatewayNetProxy::OnClientDisconnect(const uint8_t nType, const MPSOCK nSockIndex)
{
	auto pNetModule = GetNetModule(nType);
	pNetModule->DelNetObject(nSockIndex);
	switch (nType)
	{
	case MP_CLIENT:
	{
		auto pGateUserMgr = GetModule<GateUserManager>(eGateMgr_GateUser);
		pGateUserMgr->GateUserDisConnect(nSockIndex);
	}
	break;
	case MP_ST_GAME:
	{
		auto pGameServerMgr = GetModule<GameServerManager>(eGateMgr_GameServer);
		pGameServerMgr->DelGameServer(nSockIndex);

		MP_DEBUG("Can Not Connect To GameServer!");

		auto pGateUserMgr = GetModule<GateUserManager>(eGateMgr_GateUser);
		pGateUserMgr->KickAll();
	}
	break;
	case MP_ST_SUPER:
	{
		auto pSuperServerMgr = GetModule<SuperServerManager>(eGateMgr_SuperServer);
		pSuperServerMgr->DelSuperServer(nSockIndex);
		MP_DEBUG("Can Not Connect To SuperServer!");
	}
	break;
	default:
		break;
	}
}

void GatewayNetProxy::OnClientConnected(const uint8_t nType, const MPSOCK nSockIndex)
{
	auto pNetModule = GetNetModule(nType);
	auto pNetObject = pNetModule->GetNetObject(nSockIndex);
	if (pNetObject == nullptr)
	{
		MP_ERROR("Error GateUser!");
		return;
	}
	switch (nType)
	{
	case MP_CLIENT:
	{
		auto sIP = pNetObject->GetIP();
		auto pGateUserMgr = GetModule<GateUserManager>(eGateMgr_GateUser);
		pGateUserMgr->GateUserConnect(nSockIndex, sIP);
	}
	break;
	case MP_ST_GAME:
	{
		auto pGameServerMgr = GetModule<GameServerManager>(eGateMgr_GameServer);
		pGameServerMgr->AddGameServer(nSockIndex, pNetObject->GetIP().c_str(), pNetObject->GetPort());
	}
	break;
	case MP_ST_SUPER:
	{
		auto pSuperServerMgr = GetModule<SuperServerManager>(eGateMgr_SuperServer);
		pSuperServerMgr->AddSuperServer(nSockIndex, pNetObject->GetIP().c_str(), pNetObject->GetPort());
	}
	break;
	}
}

const MPGUID GatewayNetProxy::GetGUIDBySock(const uint8_t nType,const MPSOCK nSockIndex)const
{
	switch (nType)
	{
	case MP_CLIENT:
	{
		auto pGateUserMgr = GetModule<GateUserManager>(eGateMgr_GateUser);
		return pGateUserMgr->GetGUID(nSockIndex);
	}
	break;
	case MP_ST_GAME:
	{
		auto pGameServerMgr = GetModule<GameServerManager>(eGateMgr_GameServer);
		return 0;
	}
	break;
	case MP_ST_SUPER:
	{
		auto pSuperServerMgr = GetModule<SuperServerManager>(eGateMgr_SuperServer);
		return 0;
	}
	break;
	default:
		break;
	}
	return 0;
}
