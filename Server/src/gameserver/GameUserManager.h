#pragma once
#include "GameCommDef.h"
#include <unordered_map>
#include "ManagerModule.h"
#include "GameUser.h"
#include "ModuleTimeTick.h"

typedef std::function<void(GameUserPtr, const std::string&)> USER_CB_FUNCTOR;
typedef std::shared_ptr<USER_CB_FUNCTOR> USER_CB_FUNCTOR_PTR;

class GameUserManager : public ManagerModule,public ModuleTimeTick
{
public:
	GameUserManager();
	~GameUserManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	template<typename BaseType>
	bool AddReceiveCallBack(const uint32_t nType, BaseType* pBase, const uint16_t nMsgId, void(BaseType::*handleRecieve)(GameUserPtr, const std::string&))
	{
		auto f = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		USER_CB_FUNCTOR_PTR pFunc(new USER_CB_FUNCTOR(f));
		return m_mUserCBs.emplace(nMsgId, pFunc).second;
	}

	void UserMsgDispatch(
		const MPGUID uid,
		uint64_t nUserSockIndex,
		uint64_t nGateSockIndex,
		const uint32_t nMsgId,
		const std::string& sData
	);

	void SendMsg(const MPGUID uid,google::protobuf::Message& pb);
	void SendMsgAll(google::protobuf::Message& pb);

	GameUserPtr GetGameUser(const MPGUID uid);

	void GameUserLogon(GameUserPtr pGameUser);
	void GameUserLogout(const MPGUID uid);

	void KickAllByGateSock(const uint64_t nGateSock);

	void GameUserModuleRefresh(uint32_t nModuleIndex);
private:
	void addGameUser(GameUserPtr pGameUser);
	void delGameUser(const MPGUID uid);

	GameUserPtr checkGetGameUser(const MPGUID uid, uint64_t nUserSock,uint64_t nGateSock);
	GameUserPtr createGameUser(
		uint64_t nUserSock,
		uint64_t nGateSock,
		const std::string& sAccount,
		const std::string& sPassword
	);

	void onGameUserLogon(
		GameUserPtr pGameUser,
		uint64_t nUserSockIndex,
		uint64_t nGateSockIndex,
		const std::string& sData
		);

	void onGameUserLogonSuccess(GameUserPtr pGameUser);

	int loadGameUser(GameUserPtr pGameUser);
	void saveGameUserLogon(GameUserPtr pGameUser);

private:

	std::map<uint32_t, USER_CB_FUNCTOR_PTR> m_mUserCBs;

	MPMemoryPool<GameUser> m_GameUserPool;

	std::unordered_map<MPGUID, GameUserPtr> m_mGameUsers;

	std::mutex mtx;
};

GAME_MANAGER_MODULE_REG(eGameMgr_GameUser, GameUserManager);