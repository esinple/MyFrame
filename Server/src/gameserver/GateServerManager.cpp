#include "GateServerManager.h"
#include "MPDefine.pb.h"
#include "MPDefine.pb.h"
#include "Gate2Game.pb.h"
#include "MPLogger.h"
#include "GameUserManager.h"

CPP_GAME_MANAGER_REG(GateServerManager);

GateServerManager::GateServerManager()
	: ManagerModule(eGameMgr_GateServer)
{
}

GateServerManager::~GateServerManager()
{
}
bool GateServerManager::Awake()
{
	REGISTER_GATE(this, MPMsg::eGameMsg_GameMsgWrapper, GateServerManager::OnGateUserMsg);
	REGISTER_GATE(this, MPMsg::eGameMsg_GateUserLogout, GateServerManager::OnGateUserLogout);
	return true;
}

bool GateServerManager::AfterAwake()
{
	return true;
}

bool GateServerManager::Execute()
{
	return true;
}

bool GateServerManager::BeforeShutDown()
{
	return true;
}

bool GateServerManager::ShutDown()
{
	return true;
}

void GateServerManager::AddGateServer(uint64_t nSockIndex, const char* ip, int nPort)
{
	auto pGateServer = std::make_shared<GateServer>(nSockIndex, ip, nPort);
	if (pGateServer == nullptr)
	{
		return;
	}
	
	if (!m_mGateServers.emplace(nSockIndex, pGateServer).second)
	{
		MP_ERROR("Same Gate Server!");
	}
}

void GateServerManager::DelGateServer(uint64_t nSockIndex)
{
	m_mGateServers.erase(nSockIndex);
}

GateServerPtr GateServerManager::GetGateServer(uint64_t nSockIndex)
{
	auto it = m_mGateServers.find(nSockIndex);
	if (it == m_mGateServers.end())
	{
		return nullptr;
	}
	return it->second;
}

void GateServerManager::OnGateUserMsg(const std::string& sData, const uint64_t nSockIndex)
{
	MPMsg::GateUserWrapper_Gate2Game inMsg;
	inMsg.ParseFromString(sData);

	MPMsg::MsgBase msg;
	if (!msg.ParseFromString(inMsg.raw_data()))
	{
		return;
	}

	auto pGameUserMgr = g_pGameNetProxy->GetModule<GameUserManager>(eGameMgr_GameUser);
	pGameUserMgr->UserMsgDispatch(inMsg.user_id(), inMsg.fd(), nSockIndex, msg.msg_id(), msg.msg_data());
}

void GateServerManager::OnGateUserLogout(const std::string& sData, const uint64_t nSockIndex)
{
	MPMsg::GateUserLogout_Gate2Game inMsg;
	if (!inMsg.ParseFromString(sData))
	{
		return;
	}
	auto pGameUserMgr = g_pGameNetProxy->GetModule<GameUserManager>(eGameMgr_GameUser);
	pGameUserMgr->GameUserLogout(inMsg.user_id());
}

void GateServerManager::SendToGate(uint64_t nSockIndex, uint32_t nMsgId,google::protobuf::Message& pb)
{
	auto pGateServer = GetGateServer(nSockIndex);
	if (pGateServer == nullptr)
	{
		//error;
		return;
	}

	SEND_USER_MSG(nSockIndex, nMsgId, pb);
}

void GateServerManager::SendToGateAll(uint32_t nMsgId,google::protobuf::Message& pb)
{
	for (auto& gi : m_mGateServers)
	{
		SEND_USER_MSG(gi.first, nMsgId, pb);
	}
}