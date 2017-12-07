#include "GameUser.h"
#include "GateServerManager.h"
#include "GameNetProxy.h"
#include "Gate2Game.pb.h"

GameUser::GameUser(const MPGUID uid, uint64_t nUserSock, uint64_t nGateSock)
	: m_UserModules(eGameUser_End)
{
	ReuseInit(uid, nUserSock, nGateSock);
}

GameUser::~GameUser()
{
}

void GameUser::ReuseInit(const MPGUID uid, uint64_t nUserSock, uint64_t nGateSock)
{
	m_uid = uid;
	m_nUserSock = nUserSock;
	m_nGateSock = nGateSock;
}

void GameUser::Clear()
{
}

MPGUID GameUser::GetUID()const
{
	return m_uid;
}

uint64_t GameUser::GetUserSock()const
{
	return m_nUserSock;
}

uint64_t GameUser::GetGateSock()const
{
	return m_nGateSock;
}

void GameUser::Send(uint32_t nMsgId,google::protobuf::Message& msg)
{
	MPMsg::MsgBase base;
	base.set_msg_type(MPMsg::eMsgType_Client);
	base.set_msg_id(nMsgId);
	base.set_msg_data(msg.SerializeAsString());
	auto sBaseData = base.SerializeAsString();

	std::string strOutData;
	MPNet::EnCode(nMsgId, sBaseData.c_str(), (uint32_t)sBaseData.size(), strOutData);

	MPMsg::GateUserWrapper_Game2Gate outMsg;
	outMsg.set_user_id(GetUID());
	outMsg.set_raw_data(strOutData);

	SEND2GATE(GetGateSock(), MPMsg::eGameMsg_GameMsgWrapper, outMsg);
}

void GameUser::LoadFromDB()
{
}