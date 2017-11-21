#pragma once
#include "NetProxy.h"
#include "MPThread.h"
#include "ManagerModuleManager.h"
#include "CommDef.h"
#include "MPSingleton.h"

class SuperNetProxy final : public NetProxy
{
	MP_SINGLETON_DECLARE(SuperNetProxy);
public:
	virtual bool InitServerCfg(const std::string& filename)override;
	virtual void OnClientDisconnect(const uint8_t nType, const MPSOCK nSockIndex)override;
	virtual void OnClientConnected(const uint8_t nType, const MPSOCK nSockIndex)override;
	virtual void LogicStart()override;
	virtual void LogicFinal()override;
	virtual void LogicRun()override;
private:
	ManagerModuleManager m_Mgrs;
};

#define g_pNetProxy SuperNetProxy::GetInstance()