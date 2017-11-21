#include "SuperNetProxy.h"
#include "SuperManagerModuleEnum.h"
#include "SuperManagerModuleDefine.h"
#include "CommDef.h"

MP_SINGLETON_IMPLEMENT(SuperNetProxy);

SuperNetProxy::SuperNetProxy() 
	: NetProxy(100),m_Mgrs(eSuperManagerModule_End,GetSuperMgrModuleName)
{
}

SuperNetProxy::~SuperNetProxy()
{
}

bool SuperNetProxy::InitServerCfg(const std::string& filename)
{
	AddServerNetModule(MP_ST_GAME, 3000, 16123);
	AddServerNetModule(MP_ST_GATE, 3000, 16124);

	//AddReceiveCallBack();
	//AddServerNetModule(MP_ST_CENTER, 3000, 16125);
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
}

void SuperNetProxy::OnClientDisconnect(const uint8_t nType, const MPSOCK nSockIndex)
{
}

void SuperNetProxy::OnClientConnected(const uint8_t nType, const MPSOCK nSockIndex)
{
}
