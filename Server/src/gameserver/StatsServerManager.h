#pragma once
#include "GameCommDef.h"
#include "ManagerModule.h"
#include "StatsServer.h"

class StatsServerManager : public ManagerModule
{
public:
	StatsServerManager();
	~StatsServerManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	void AddStatsServer(uint64_t nSockIndex, const char* ip, int nPort);
	void DelStatsServer(uint64_t nSockIndex);
private:
private:
	MPMemoryPool<StatsServer> m_StatsServerPool;

	std::map<const uint64_t, StatsServerPtr> m_mStatsServers;
	std::map<const uint64_t, StatsServerPtr> m_mTempStatsServers;
};

GAME_MANAGER_MODULE_REG(eGameMgr_StatsServer, StatsServerManager);