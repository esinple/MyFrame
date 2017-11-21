#pragma once
#include "NetProxy.h"
#include "MPThread.h"
#include "ManagerModuleManager.h"
#include "CommDef.h"
#include "MPSingleton.h"

class StatsNetProxy final : public NetProxy
{
	MP_SINGLETON_DECLARE(StatsNetProxy);
public:
	virtual bool InitServerCfg(const std::string& filename)override;
	virtual void OnClientDisconnect(const uint8_t nType, const MPSOCK nSockIndex)override;
	virtual void OnClientConnected(const uint8_t nType, const MPSOCK nSockIndex)override;
	virtual void LogicStart()override;
	virtual void LogicFinal()override;
	virtual void LogicRun()override;
	virtual const MPGUID GetGUIDBySock(const uint8_t nType,const MPSOCK nSockIndex)const override;

	template<typename T>
	T* GetModule(uint32_t nType)
	{
		return m_Mgrs.GetModule<T>(nType);
	}
private:
	ManagerModuleManager m_Mgrs;
};

#define g_pStatsNetProxy StatsNetProxy::GetInstance()