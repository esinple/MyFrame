#pragma once
#include "ManagerModule.h"
#include "GatewayCommDef.h"
#include "MPMemoryPool.h"
#include "GameServer.h"

class GameServerManager : public ManagerModule
{
public:
	GameServerManager();
	~GameServerManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	void AddGameServer(uint64_t nSockIndex,const char* ip,int nPort);
	void DelGameServer(uint64_t nSockIndex);

private:
	H_GATE_MANAGER_REG(GameServerManager);

	MPMemoryPool<GameServer> m_GameServerPool;

	std::map<const uint64_t, GameServerPtr> m_mGameServers;
	std::map<const uint64_t, GameServerPtr> m_mTempGameServers;
};