#include "GameUserManager.h"
#include "GateServerManager.h"
#include "MPDefine.pb.h"
#include "Gate2Game.pb.h"
#include "UserMsgDefine.pb.h"
#include "UserBase.pb.h"

CPP_GAME_MANAGER_REG(GameUserManager)

GameUserManager::GameUserManager()
	: ManagerModule(eGameMgr_GameUser)
{
	AddParent(eGameMgr_Time);
	AddParent(eGameMgr_DBServer);
}

GameUserManager::~GameUserManager()
{
}
bool GameUserManager::Awake()
{
	//REGISTER_SUPER(this, MPMsg::eGate2Stats_VerifyMe, GameUserManager::OnStatsVerify);

	return true;
}
bool GameUserManager::AfterAwake()
{
	return true;
}
bool GameUserManager::Execute()
{
	return true;
}
bool GameUserManager::BeforeShutDown()
{
	return true;
}
bool GameUserManager::ShutDown()
{
	return true;
}

void GameUserManager::addGameUser(GameUserPtr pGameUser)
{
	MP_DEBUG("Add GameUser [%llu]",pGameUser->GetUID());

	m_mGameUsers.emplace(pGameUser->GetUID(), pGameUser);
}

void GameUserManager::delGameUser(const MPGUID uid)
{
	MP_DEBUG("Del GameUser [%llu]", uid);

	m_mGameUsers.erase(uid);
}

GameUserPtr GameUserManager::GetGameUser(const MPGUID uid)
{
	auto it = m_mGameUsers.find(uid);
	if (it == m_mGameUsers.end())
	{
		return nullptr;
	}
	return it->second;
}

GameUserPtr GameUserManager::checkGetGameUser(const MPGUID uid, uint64_t nUserSock,const uint64_t nGateSock)
{
	auto pGameUser = GetGameUser(uid);
	if (pGameUser == nullptr)
	{
		//MP_ERROR("GameUser [%llu] Sock[%llu] Not Found!", uid, nUserSock);
		return nullptr;
	}
	if (pGameUser->GetUserSock() != nUserSock || pGameUser->GetGateSock() != nGateSock)
	{
		MP_ERROR("GameUser [%llu] Sock[%llu] Same UID Old[%llu]!", uid, nUserSock,pGameUser->GetUserSock());
		return nullptr;
	}

	return pGameUser;
}

void GameUserManager::UserMsgDispatch(
	const MPGUID uid,
	uint64_t nUserSockIndex,
	uint64_t nGateSockIndex,
	const uint32_t nMsgId,
	const std::string& sData
)
{
	auto pGameUser = checkGetGameUser(uid, nUserSockIndex, nGateSockIndex);
	switch (nMsgId)
	{
	case UserMsg_Logon:
	{
		onGameUserLogon(pGameUser, nUserSockIndex, nGateSockIndex, sData);
	}
	break;
	default:
	{
		auto it = m_mUserCBs.find(nMsgId);
		if (it == m_mUserCBs.end())
		{
			return;
		}

		it->second->operator()(pGameUser, sData);
	}
	break;
	}
}

void GameUserManager::onGameUserLogon(
	GameUserPtr pGameUser,
	uint64_t nUserSockIndex,
	uint64_t nGateSockIndex,
	const std::string& sData)
{
	if (pGameUser != nullptr)
	{
		//ÌßµôÔ­À´µÄGameUser
	}

	//find from db

	bool bFound(false);
	//not found!
	if (!bFound)
	{
		UserLogon_C2S inMsg;
		if (!inMsg.ParseFromString(sData))
		{
			MP_ERROR("UserLogon Error! User [%llu],Gate [%llu]", nUserSockIndex, nGateSockIndex);
			return;
		}

		pGameUser = createGameUser(nUserSockIndex, nGateSockIndex, inMsg.account(), inMsg.password());
		if (pGameUser == nullptr)
		{
			MP_ERROR("Can Not Create GameUser! [%llu][%llu],Account[%s]Password[%s]!"
				, nUserSockIndex, nGateSockIndex, inMsg.account(), inMsg.password());
			return;
		}

		addGameUser(pGameUser);
	}

	GameUserLogon(pGameUser);

	//notify gate
	onGameUserLogonSuccess(pGameUser);
}

void GameUserManager::onGameUserLogonSuccess(GameUserPtr pGameUser)
{
	//notify gate
	MPMsg::GameUserLogonSuccess_Game2Gate outGateMsg;
	outGateMsg.set_fd(pGameUser->GetUserSock());
	outGateMsg.set_user_id(pGameUser->GetUID());
	SEND2GATE(pGameUser->GetGateSock(), MPMsg::eGameMsg_GameUserLogonSuccess,outGateMsg);

	//notify user
	UserLogon_S2C outUserMsg;
	outUserMsg.set_errcode(UEC_Success);
	outUserMsg.set_user_id(pGameUser->GetUID());
	outUserMsg.set_account(pGameUser->GetAccount());
	pGameUser->Send(UserMsg_Logon,outUserMsg);
}

GameUserPtr GameUserManager::createGameUser(
	uint64_t nUserSock,
	uint64_t nGateSock,
	const std::string& sAccount,
	const std::string& sPassword
)
{
	auto uid = GenerateNewGUID(1);
	auto pGameUser = m_GameUserPool.Get(uid, nUserSock, nGateSock);
	pGameUser->SetAccount(sAccount);
	pGameUser->SetPassword(sPassword);
	return pGameUser;
}

void GameUserManager::GameUserLogon(GameUserPtr pGameUser)
{
	MP_INFO("GameUser Logon! uid : [%llu]",pGameUser->GetUID());
	if (!pGameUser->GetAllModules().Awake())
	{
		//error
	}
}

void GameUserManager::GameUserLogout(const MPGUID uid)
{
	MP_INFO("GameUser Logout! uid : [%llu]", uid);
	//logic??
	auto pGameUser = GetGameUser(uid);
	if (pGameUser == nullptr)
	{
		return;
	}

	pGameUser->GetAllModules().ShutDown();

	//delete
	delGameUser(uid);
}