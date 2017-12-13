#pragma once
#include "GameCommDef.h"
#include "GameUser.h"
#include "RefreshT.h"

class RefreshManager : public ManagerModule
{
public:
	RefreshManager();
	~RefreshManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	const UserRefreshCfg* const GetUserRefreshCfg(uint64_t nId)const;

	void GameUserLoginRefresh(GameUserPtr pGameUser);

	void GameUserOnlineRefresh(uint64_t id);

private:
	bool loadUserRefreshCfg();

	bool calcRefreshTime(UserRefreshCfg* pCfg);

	bool calcRefreshOffset(UserRefreshCfg* pCfg,uint32_t& nOffset,uint32_t& nIntervalSec);

	void registerUserAllRefresh();

	void registerUserRefresh(UserRefreshCfg* pCfg);

	UserRefreshCfg* getUserRefreshCfg(uint64_t nId);
private:
	std::unordered_map<uint64_t, UserRefreshCfg> m_mUserRefreshCfgs;
};

GAME_MANAGER_MODULE_REG(eGameMgr_Refresh, RefreshManager);