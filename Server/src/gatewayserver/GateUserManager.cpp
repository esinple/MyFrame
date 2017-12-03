#include "GateUserManager.h"
#include "GatewayNetProxy.h"
#include "GateTimeManager.h"
#include "Gate2Game.pb.h"
#include "GameServerManager.h"
#include "UserBase.pb.h"
#include "MPTCPServer.h"

CPP_GATE_MANAGER_REG(GateUserManager);

GateUserManager::GateUserManager() 
	: ManagerModule(eGateMgr_GateUser)
{
}

GateUserManager::~GateUserManager()
{
}

bool GateUserManager::Awake()
{
	REGISTER_CLIENT(this, GateUserManager::OnUser2GameMsgWrapperSock);
	REGISTER_CLIENT(this, GateUserManager::OnUser2GameMsgWrapperUID);
	REGISTER_GAME(this, MPMsg::eGameMsg_GameMsgWrapper, GateUserManager::OnGame2UserMsgWrapperSock);
	REGISTER_GAME(this, MPMsg::eGameMsg_GameMsgWrapper, GateUserManager::OnGame2UserMsgWrapperUID);
	REGISTER_GAME(this, MPMsg::eGameMsg_GameUserLogonSuccess, GateUserManager::OnGameUserLogonSuccess);
	return true;
}

bool GateUserManager::AfterAwake()
{
	return true;
}

bool GateUserManager::Execute()
{
	checkTempGateUsers();

	return true;
}
bool GateUserManager::BeforeShutDown()
{
	return true;
}

bool GateUserManager::ShutDown()
{
	return true;
}

void GateUserManager::GateUserConnect(uint64_t nSockIndex, const std::string& sIP)
{
	addTempGateUser(nSockIndex, sIP);
}

void GateUserManager::GateUserDisConnect(uint64_t nSockIndex)
{
	if (GetTempGateUser(nSockIndex) == nullptr)
	{
		auto uid = GetGUID(nSockIndex);
		delGateUser(uid);
		onGateUserDisConnect(uid);
	}
	else
	{
		delTempGateUser(nSockIndex);
	}
}

void GateUserManager::addTempGateUser(uint64_t nSockIndex, const std::string& sIP)
{
	auto pGateUser = m_objGateUserPool.Get(nSockIndex,sIP);
	if (pGateUser == nullptr)
	{
		MP_ERROR("Cannot Create GateUser! SockIndex [%lld],ip [%s]",nSockIndex,sIP.c_str());
		return;
	}
	pGateUser->SetLoginTime(GATE_CUR_TIME);

	mtx.lock();
	if (!m_mTempGateUsers.emplace(nSockIndex, pGateUser).second)
	{
		mtx.unlock();
		MP_ERROR("Same Sock Index!");
		return;
	}
	mtx.unlock();

	MP_DEBUG("New Temp GateUser Connected! ip [%lld][%s]",nSockIndex,sIP.c_str());
}

void GateUserManager::delTempGateUser(uint64_t nSockIndex)
{
	mtx.lock();
	m_mTempGateUsers.erase(nSockIndex);
	mtx.unlock();
	MP_DEBUG("Del Temp GateUser ! fd [%lld]", nSockIndex);
}

GateUserPtr GateUserManager::GetTempGateUser(uint64_t nSockIndex)
{
	auto it = m_mTempGateUsers.find(nSockIndex);
	if (it == m_mTempGateUsers.end())
	{
		return nullptr;
	}
	return it->second;
}

const MPGUID GateUserManager::GetGUID(uint64_t nSockIndex)const
{
	auto it = m_mSock2GUID.find(nSockIndex);
	if (it == m_mSock2GUID.end())
	{
		return 0;
	}
	return it->second;
}

GateUserPtr GateUserManager::GetGateUser(MPGUID uid)
{
	auto it = m_mGateUsers.find(uid);
	if (it == m_mGateUsers.end())
	{
		return nullptr;
	}
	return it->second;
}

void GateUserManager::addGateUser(MPGUID uid, uint64_t nSockIndex, GateUserPtr pGateUser)
{
	m_mSock2GUID.emplace(nSockIndex, uid);

	m_mGateUsers.emplace(uid, pGateUser);

	MP_DEBUG("Add GateUser!fd [%d],uid [%llu]!", nSockIndex, uid);
}

void GateUserManager::delGateUser(MPGUID uid)
{
	auto pGateUser = GetGateUser(uid);
	if (pGateUser == nullptr)
	{
		return;
	}
	MP_DEBUG("Del GateUser!fd [%d],uid [%llu]!", pGateUser->GetFD(), uid);
	m_mSock2GUID.erase(pGateUser->GetFD());
	m_mGateUsers.erase(uid);

}

void GateUserManager::checkTempGateUsers()
{
	mtx.lock();
	auto it = m_mTempGateUsers.begin();
	while (it != m_mTempGateUsers.end())
	{
		if (it->second->GetLoginTime() + 60 > GATE_CUR_TIME)
		{
			++it;
			continue;
		}
		//连接超过60秒无反应
		auto nSockIndex = it->second->GetFD();
		MP_WARN("Kick Unknown Temp GateUser![%lld]", nSockIndex);
		it = m_mTempGateUsers.erase(it);

		g_pGatewayNetProxy->Kick(MP_CLIENT, nSockIndex,"Temp GateUser TimeOut!");
	}
	mtx.unlock();
}

bool GateUserManager::OnUser2GameMsgWrapperSock(MPMsg::MsgBase& msg_base, const uint64_t nSockIndex)
{
	MPMsg::GateUserWrapper_Gate2Game msg;
	msg.set_fd(nSockIndex);
	msg.set_user_id(GetGUID(nSockIndex));
	msg.set_raw_data(msg_base.SerializeAsString());

	SEND2GAME_ALL(MPMsg::eGameMsg_GameMsgWrapper,msg);
	return true;
}

bool GateUserManager::OnUser2GameMsgWrapperUID(MPGUID uid, MPMsg::MsgBase& msg_base)
{
	auto pGateUser = GetGateUser(uid);
	if (pGateUser == nullptr)
	{
		return false;
	}

	MPMsg::GateUserWrapper_Gate2Game msg;
	msg.set_fd(pGateUser->GetFD());
	msg.set_user_id(uid);
	msg.set_raw_data(msg_base.SerializeAsString());

	SEND2GAME_ALL(MPMsg::eGameMsg_GameMsgWrapper,msg);
	return true;
}

void GateUserManager::OnGame2UserMsgWrapperSock(const std::string& data, const uint64_t nSockIndex)
{
	MPMsg::GateUserWrapper_Game2Gate msg;
	if (!msg.ParseFromString(data))
	{
		return;
	}

	auto pGateUser = GetGateUser(msg.user_id());
	if (pGateUser == nullptr)
	{
		return;
	}

	SEND2CLIENT_NOMSGID(pGateUser->GetFD(), msg.raw_data());
}

void GateUserManager::OnGame2UserMsgWrapperUID(MPGUID uid, const std::string& data)
{
	MPMsg::GateUserWrapper_Game2Gate msg;
	if (!msg.ParseFromString(data))
	{
		return;
	}

	auto pGateUser = GetGateUser(uid);
	if (pGateUser == nullptr)
	{
		return;
	}

	SEND2CLIENT_NOMSGID(pGateUser->GetFD(), msg.raw_data());
}

void GateUserManager::OnGameUserLogonSuccess(const std::string& data, const uint64_t nSockIndex)
{
	MP_DEBUG("GameUser Logon Success!");
	MPMsg::GameUserLogonSuccess_Game2Gate inMsg;
	if (!inMsg.ParseFromString(data))
	{
		return;
	}

	auto pGateUser = GetTempGateUser(inMsg.fd());
	if (pGateUser == nullptr)
	{
		onGateUserDisConnect(inMsg.user_id());
		return;
	}

	delTempGateUser(inMsg.fd());
	addGateUser(inMsg.user_id(), inMsg.fd(),pGateUser);
}

void GateUserManager::onGateUserDisConnect(MPGUID uid)
{
	MPMsg::GateUserLogout_Gate2Game outMsg;
	outMsg.set_user_id(uid);
	SEND2GAME_ALL(MPMsg::eGameMsg_GateUserLogout, outMsg);
}


void GateUserManager::KickAll()
{
	auto mGateUsers = m_mGateUsers;
	for (auto&&[uid, pGateUser] : mGateUsers)
	{
		g_pGatewayNetProxy->Kick(MP_CLIENT, pGateUser->GetFD(),"GameServer Offline");
	}
	m_mGateUsers.clear();

	auto mTempGateUsers = m_mTempGateUsers;
	for (auto&&[nSockIndex, pGateUser] : mTempGateUsers)
	{
		g_pGatewayNetProxy->Kick(MP_CLIENT, nSockIndex,"GameServer Offline");
	}
	m_mTempGateUsers.clear();

	m_mSock2GUID.clear();
}