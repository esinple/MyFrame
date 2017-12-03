#include "GameUserManager.h"
#include "GateServerManager.h"
#include "MPDefine.pb.h"
#include "Gate2Game.pb.h"
#include "UserMsgDefine.pb.h"
#include "UserBase.pb.h"
#include "DBManager.h"
#include "UserBase.h"
#include "GameTimeManager.h"

CPP_GAME_MANAGER_REG(GameUserManager)

GameUserManager::GameUserManager()
	: ManagerModule(eGameMgr_GameUser),ModuleTimeTick()
{
	AddParent(eGameMgr_Time);
	AddParent(eGameMgr_DBServer);
	AddParent(eGameMgr_TimeStamp);
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
//	if (Tick(GAME_CUR_TIME, std::chrono::minutes(1)))
	if(_1_SEC_TICK_)
	{
		MP_DEBUG("Cur GameUser : %d", m_mGameUsers.size());
	}

	//测试提交更改
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

void GameUserManager::onGameUserLogon(
	GameUserPtr pGameUser,
	uint64_t nUserSockIndex,
	uint64_t nGateSockIndex,
	const std::string& sData)
{
	if (pGameUser != nullptr)
	{
		//踢掉原来的GameUser
	}

	UserLogon_C2S inMsg;
	if (!inMsg.ParseFromString(sData))
	{
		MP_ERROR("UserLogon Error! User [%llu],Gate [%llu]", nUserSockIndex, nGateSockIndex);
		return;
	}
	//构造gameuser
	pGameUser = createGameUser(nUserSockIndex, nGateSockIndex, inMsg.account(), inMsg.password());
	if (pGameUser == nullptr)
	{
		MP_ERROR("Can Not Create GameUser! [%llu][%llu],Account[%s]Password[%s]!"
			, nUserSockIndex, nGateSockIndex, inMsg.account(), inMsg.password());
	}
	
	//加载gameuser 如果有数据
	auto nErrCode = loadGameUser(pGameUser);
	if (nErrCode != UEC_Success)
	{
		MP_ERROR("UserLogon Password Error! User [%llu],Gate [%llu]", nUserSockIndex, nGateSockIndex);
		return;
	}

	addGameUser(pGameUser);

	GameUserLogon(pGameUser);

	//notify gate
	onGameUserLogonSuccess(pGameUser);
}

void GameUserManager::onGameUserLogonSuccess(GameUserPtr pGameUser)
{
	saveGameUserLogon(pGameUser);
	
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

int GameUserManager::loadGameUser(GameUserPtr pGameUser)
{
	DBGameServer::GameUserLogon db_gameuserbase;
	db_gameuserbase.set_account(std::string(pGameUser->GetAccount()));
	
	DBGameServer::GameUserLogon db_result;
	if (g_pDBMgr->ExecSelectOne(db_gameuserbase, db_result) == 0)
	{
		return UEC_Success;
	}

	if (db_result.passwd() != pGameUser->GetPassword())
	{
		return UEC_Failed;
	}

	pGameUser->SetIP("");
	pGameUser->SetLoginTime(GAME_CUR_TIME);
	meplay::MPTime LogoutTime(db_result.logout_time());
	pGameUser->SetLogoutTime(LogoutTime);
	
	pGameUser->LoadFromDB();
	return UEC_Success;
}

void GameUserManager::saveGameUserLogon(GameUserPtr pGameUser)
{
	DBGameServer::GameUserLogon db_logon_info_filter;
	db_logon_info_filter.set_account(pGameUser->GetAccount());

	DBGameServer::GameUserLogon db_logon_info;
	db_logon_info.set_account(pGameUser->GetAccount());
	db_logon_info.set_passwd(pGameUser->GetPassword());
	db_logon_info.set_uid(pGameUser->GetUID());
	db_logon_info.set_ip(pGameUser->GetIP());
	db_logon_info.set_login_time(pGameUser->GetLoginTime().CurrentSec());
	db_logon_info.set_logout_time(pGameUser->GetLogoutTime().CurrentSec());

	SAVE2DB(db_logon_info_filter,db_logon_info);
}

void GameUserManager::KickAllByGateSock(const uint64_t nGateSock)
{
	std::vector<MPGUID> vKickUIDs;
	for (auto&&[uid, pGameUser] : m_mGameUsers)
	{
		if (pGameUser->GetGateSock() == nGateSock)
		{
			vKickUIDs.emplace_back(uid);
		}
	}

	for (auto uid : vKickUIDs)
	{
		GameUserLogout(uid);
	}
}