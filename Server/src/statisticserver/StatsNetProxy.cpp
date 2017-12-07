#include "StatsNetProxy.h"
#include "StatsManagerModuleDefine.h"
#include "StatsCommDef.h"

MP_SINGLETON_IMPLEMENT(StatsNetProxy);

StatsNetProxy::StatsNetProxy() 
	: NetProxy(100),m_Mgrs(eStatsManagerModule,eStatsMgr_End)
{
}

StatsNetProxy::~StatsNetProxy()
{
}

bool StatsNetProxy::InitServerCfg(const std::string& filename)
{
	AddTCPServerModule(MP_ST_GAME, 3000, "0.0.0.0", 16123);
	AddTCPServerModule(MP_ST_GATE, 3000, "0.0.0.0", 16124);

	//AddReceiveCallBack();
	//AddTCPServerModule(MP_ST_CENTER, 3000, 16125);
	return true;
}

void StatsNetProxy::LogicStart()
{
	m_Mgrs.Awake();
}
void StatsNetProxy::LogicFinal()
{
	m_Mgrs.ShutDown();
}

void StatsNetProxy::LogicRun()
{
	m_Mgrs.Execute();
}

void StatsNetProxy::OnClientDisconnect(const uint8_t nType, const MPSOCK nSockIndex)
{
	switch (nType)
	{
	case MP_ST_GAME:
	{

	}
	break;
	case MP_ST_GATE:
	break;
	case MP_ST_CENTER:
	break;
	default:
		break;
	}
}

void StatsNetProxy::OnClientConnected(const uint8_t nType, const MPSOCK nSockIndex)
{
	switch (nType)
	{
	case MP_ST_GAME:
	{
		
	}
	break;
	case MP_ST_GATE:
		break;
	case MP_ST_CENTER:
		break;
	default:
		break;
	}
}

const MPGUID StatsNetProxy::GetGUIDBySock(const uint8_t nType, const MPSOCK nSockIndex)const
{
	return 0;
}