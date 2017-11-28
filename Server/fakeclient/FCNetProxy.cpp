#include "FCNetProxy.h"
#include "FCManagerModuleDefine.h"
#include "FCManagerModuleEnum.h"
#include "TestManager.h"

MP_SINGLETON_IMPLEMENT(FCNetProxy);

#define REG_MODULE_NAME(e,c) case e:return c;

const char * GetFCMgrModuleName(uint32_t type)
{
	switch (type)
	{
		REG_MODULE_NAME(eFCMgr_Test, "TestManager");
	default:
		return nullptr;
	}
};

FCNetProxy::FCNetProxy() 
	: NetProxy(100), m_Mgrs(eFCMgr_End, GetFCMgrModuleName)
{
}

FCNetProxy::~FCNetProxy()
{
}

bool FCNetProxy::InitServerCfg(const std::string& filename)
{
	//AddClientNetModule(MP_ST_GATE, "127.0.0.1", 17000);
	AddTCPClientModule(MP_ST_GATE, "127.0.0.1", 17000);

	return true;
}

void FCNetProxy::LogicStart()
{
	m_Mgrs.Awake();
}
void FCNetProxy::LogicFinal()
{
	m_Mgrs.ShutDown();
}

void FCNetProxy::LogicRun()
{
	m_Mgrs.Execute();
}

void FCNetProxy::OnClientDisconnect(const uint8_t nType, const MPSOCK nSockIndex)
{
	auto pTestMgr = GetModule<TestManager>(eFCMgr_Test);
	pTestMgr->SetStatus(eCS_Unknown);
}

void FCNetProxy::OnClientConnected(const uint8_t nType, const MPSOCK nSockIndex)
{
	auto pNetModule = GetNetModule(nType);
	auto pNetObject = pNetModule->GetNetObject(nSockIndex);
	if (pNetObject == nullptr)
	{
		return;
	}
	
	auto pTestMgr = GetModule<TestManager>(eFCMgr_Test);
	pTestMgr->SetStatus(eCS_Connected);
	pTestMgr->SetSockIndex(nSockIndex);
}


const MPGUID FCNetProxy::GetGUIDBySock(const uint8_t nType, const MPSOCK nSockIndex)const
{
	return 0;
}