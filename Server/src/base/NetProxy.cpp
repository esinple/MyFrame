#include "NetProxy.h"
#include "MPLogger.h"
#include "termcolor.hpp"
#include "MPMsgBase.pb.h"
#include "MPTCPServer.h"
#include "MPTCPClient.h"
#include "CommDef.h"

NetProxy::NetProxy(const time_t nGapMilliSeconds)
	: m_mNetModules(), m_bFinal(false), m_nGapMilliSeconds(nGapMilliSeconds)
{

}

NetProxy::~NetProxy()
{
}

void NetProxy::Start()
{
	LogicStart();

	while (!IsFinal())
	{
		m_tLastTime.Now();

		NetRun();

		LogicRun();

		auto millisec = m_tLastTime.MTimeOut(m_nGapMilliSeconds);
		if (millisec < 0)
		{
			MP_WARN("Loop TimeOut %d", -millisec);
			//MP_DEBUG("mill %d", millisec);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
	}

	NetFinal();
	LogicFinal();

	//CV_NotifyAll();
}

void NetProxy::NetRun()
{
	for (auto& clients : m_mNetModules)
	{
		auto pNetModule = clients.second;
		pNetModule->Execute();
	}
}

void NetProxy::NetFinal()
{
	for (auto& clients : m_mNetModules)
	{
		auto pNetModule = clients.second;
		pNetModule->Final();
	}
}

void NetProxy::Final(const char* reason)
{
	MP_SYSTEM("NetProxy Fianl For Reason [%s].",reason);
	m_bFinal = true;
	//cv.wait(std::unique_lock<std::mutex>(mtx));
}

void NetProxy::ImmediatelyFinal(const char* reason)
{
	MP_SYSTEM("NetProxy Immediately Fianl For Reason [%s].", reason);
#ifdef WIN_SYSTEM
	for (int i = 0; i < 5; ++i)
	{
		std::cout << termcolor::red;
		printf("System ShutDown in %d...\n", 5 - i);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
#endif
	exit(0);
}

bool NetProxy::IsFinal()const
{
	return m_bFinal;
}

bool NetProxy::AddTCPServerModule(uint8_t nType, const uint32_t nMaxClient, const uint16_t nPort,const uint8_t nThreadCount)
{
	auto pNet = GetNetModule(nType);
	if (pNet == nullptr)
	{

		pNet = std::shared_ptr<MPTCPServer>(new MPTCPServer("NetProxy", nType, this, &NetProxy::OnClientConnected, &NetProxy::OnClientDisconnect, &NetProxy::OnMsgCB));
		//pNet = std::make_shared<MPNetServer>(nType,this,&NetProxy::OnRecieveMessage,&NetProxy::OnSocketClientEvent);
		if (pNet->InitializationAsServer(nMaxClient, nPort,nThreadCount) < 0)
		{
			MP_ERROR("Init ServerNetModule Error![%d][MaxClient : %d][Port : %d]", nType, nMaxClient, nPort);
			ImmediatelyFinal("Init Server Net Faild!");
			return false;
		}
		MP_SYSTEM("Init ServerNetModule Success![%d][MaxClient : %d][Port : %d]", nType, nMaxClient, nPort);
		m_mNetModules.emplace(nType, pNet);
	}
	else
	{
		MP_ERROR("Already Has ServerNetModule ![%d][MaxClient : %d][Port : %d]", nType, nMaxClient, nPort);
		ImmediatelyFinal("Init Server Net Faild!");
		return false;
	}
	return true;
}

void NetProxy::AddTCPClientModule(uint8_t nType, const char* ip, const uint16_t nPort,bool bAutoConnect)
{
	auto pNet = GetNetModule(nType);
	if (pNet == nullptr)
	{
		pNet = std::shared_ptr<MPTCPClient>(new MPTCPClient("NetProxy", nType, this, &NetProxy::OnClientConnected, &NetProxy::OnClientDisconnect, &NetProxy::OnMsgCB));
		m_mNetModules.emplace(nType, pNet);
	}

	if (pNet->InitializationAsClient(ip, nPort,bAutoConnect,3000) < 0)
	{
		MP_ERROR("Init ClientNetModule Error![%d][IP : %s][Port : %d]", nType, ip, nPort);
		return ;
	}
	
	MP_SYSTEM("Init ClientNetModule Success![%d][IP: %s][Port : %d]", nType, ip, nPort);
}

bool NetProxy::AddUDPServerModule(uint8_t nType, const uint16_t nPort, const uint8_t nThreadCount)
{
	auto pNet = GetNetModule(nType);
	if (pNet == nullptr)
	{

		pNet = std::shared_ptr<MPTCPServer>(new MPTCPServer("NetProxy", nType, this, &NetProxy::OnClientConnected, &NetProxy::OnClientDisconnect, &NetProxy::OnMsgCB));
		//pNet = std::make_shared<MPNetServer>(nType,this,&NetProxy::OnRecieveMessage,&NetProxy::OnSocketClientEvent);
		if (pNet->InitializationAsServer(0/*nMaxClient*/, nPort, nThreadCount) < 0)
		{
			MP_ERROR("Init ServerNetModule Error![%d][MaxClient : %d][Port : %d]", nType, 0/*nMaxClient*/, nPort);
			ImmediatelyFinal("Init Server Net Faild!");
			return false;
		}
		MP_SYSTEM("Init ServerNetModule Success![%d][MaxClient : %d][Port : %d]", nType, 0/*nMaxClient*/, nPort);
		m_mNetModules.emplace(nType, pNet);
	}
	else
	{
		MP_ERROR("Already Has ServerNetModule ![%d][MaxClient : %d][Port : %d]", nType, 0/*nMaxClient*/, nPort);
		ImmediatelyFinal("Init Server Net Faild!");
		return false;
	}
	return true;
}

void NetProxy::InvalidMessage(const uint8_t nType, const MPSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	//printf("NFNet || unMsgID=%d\n", nMsgID);
	MP_ERROR("Unknown Message !Type : %d , Sock : %lld ,MessageID: %d , Len : %d", nType, nSockIndex, nMsgID, nLen);
}
void NetProxy::OnRecieveMessage(const uint8_t nType,const MPSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	MPMsg::MsgBase xMsg;
	if (!xMsg.ParseFromArray(msg, nLen))
	{
		MP_ERROR("Parse Message Failed from Packet to MsgBase,Type : %d , Sock : %lld ,MessageID: %d , Len : %d", nType, nSockIndex, nMsgID, nLen);
		return;
	}

	auto uid = GetGUIDBySock(nType,nSockIndex);
	if (uid == 0)
	{
		if (!processSOCKCB(nType, nSockIndex, nMsgID, xMsg))
		{
			if (!processSOCKCBEX(nType, nSockIndex, xMsg))
			{
				MP_ERROR("Sock MsgId [%d] SOCK [%lld]not found !", nMsgID, nSockIndex);
			}
		}
	}
	else
	{
		if(!processUIDCB(nType,uid,nMsgID,xMsg))
		{
			MP_ERROR("UID MsgId [%d] SOCK [%lld]not found !", nMsgID, nSockIndex);
		}

	}
	
}

void NetProxy::OnSocketClientEvent(const uint8_t nType,const MPSOCK nSockIndex, const MP_NET_EVENT eEvent, MPNet* pNet)
{
	if (eEvent & MP_NET_EVENT_EOF)
	{
		MP_DEBUG("fd [%d][%lld] MP_NET_EVENT_EOF!Connection closed", nType,nSockIndex);
		OnClientDisconnect(nType,nSockIndex);
	}
	else if (eEvent & MP_NET_EVENT_ERROR)
	{
		MP_DEBUG("fd [%d][%lld] MP_NET_EVENT_ERROR!Got an error on the connection", nType,nSockIndex);
		OnClientDisconnect(nType,nSockIndex);
	}
	else if (eEvent & MP_NET_EVENT_TIMEOUT)
	{
		MP_DEBUG("fd [%d][%lld] MP_NET_EVENT_TIMEOUT!Got an error on the connection", nType,nSockIndex);
		OnClientDisconnect(nType,nSockIndex);
	}
	else  if (eEvent == MP_NET_EVENT_CONNECTED)
	{
		MP_DEBUG("fd [%d][%lld] MP_NET_EVENT_CONNECTED! connectioned success!", nType,nSockIndex);
		OnClientConnected(nType, nSockIndex);
	}
}

void NetProxy::OnMsgCB(const uint8_t nType, const MPSOCK nSockIndex, const char * msg, const uint32_t nLen)
{
	auto pNet = GetNetModule(nType);
	if (pNet == nullptr)
	{
		return;
	}
	auto pNetObject = pNet->GetNetObject(nSockIndex);
	if (pNetObject == nullptr)
	{
		return;
	}

	const char* pOffset(msg);
	uint32_t nOffLen(nLen);

	while (nOffLen > MPMsgHead::MP_Head::MP_HEAD_LENGTH)
	{
		MPMsgHead xHead;
		int nMsgBodyLength = pNet->DeCode(pOffset, nLen, xHead);
		if (nMsgBodyLength > 0 && xHead.GetMsgID() > 0)
		{
			OnRecieveMessage(nType,pNetObject->GetRealFD(), xHead.GetMsgID(), pOffset + MPMsgHead::MP_Head::MP_HEAD_LENGTH, nMsgBodyLength);

			nOffLen -= nMsgBodyLength + MPMsgHead::MP_Head::MP_HEAD_LENGTH;
			pOffset += nMsgBodyLength + MPMsgHead::MP_Head::MP_HEAD_LENGTH;
#ifdef SSJ_DEBUG
			MP_DEBUG("[%s] Recieve Msg [%d] Success! BodyLength is %d", ServerTypeNames[nType], xHead.GetMsgID(), nMsgBodyLength);
#endif
		}
		else
		{
#ifdef SSJ_DEBUG
			MP_DEBUG("[%s] Recieve Msg Failed! BodyLength is %d", ServerTypeNames[nType], nMsgBodyLength);
#endif
		}
	}

	/*MPMsg::MsgBase xMsg;
	if (!xMsg.ParseFromArray(msg, nLen))
	{
		MP_ERROR("Parse Message Failed from Packet to MsgBase,Type : %d , Sock : %lld ,MessageID: %d , Len : %d", nType, nSockIndex, nMsgID, nLen);
		return;
	}

	auto uid = GetGUIDBySock(nType, nSockIndex);
	if (uid == 0)
	{
		if (!processSOCKCB(nType, nSockIndex, nMsgID, xMsg))
		{
			if (!processSOCKCBEX(nType, nSockIndex, xMsg))
			{
				MP_ERROR("Sock MsgId [%d] SOCK [%lld]not found !", nMsgID, nSockIndex);
			}
		}
	}
	else
	{
		if (!processUIDCB(nType, uid, nMsgID, xMsg))
		{
			MP_ERROR("UID MsgId [%d] SOCK [%lld]not found !", nMsgID, nSockIndex);
		}

	}*/
}

std::shared_ptr<MPNet> NetProxy::GetNetModule(uint32_t nType)
{
	auto it = m_mNetModules.find(nType);
	if (it != m_mNetModules.end())
	{
		return it->second;
	}
	return nullptr;
}

bool NetProxy::SendMsg(const MPMsg::MsgType nMsgType,const uint8_t from,const uint8_t to, const MPSOCK nSockIndex, const uint16_t nMsgId,const google::protobuf::Message& msg)
{
	auto pNet = GetNetModule(to);
	if (pNet == nullptr)
	{
		MP_ERROR("SendMsg Error!NetModule [%d] Not Found!", to);
		return false;
	}

	MPMsg::MsgBase msg_base;
	msg_base.set_msg_type(nMsgType);
	msg_base.set_msg_id(nMsgId);
	msg_base.set_msg_data(msg.SerializeAsString());
	auto s = msg_base.SerializeAsString();
#ifdef SSJ_DEBUG
	MP_DEBUG("[%s] Send Msg [%d] Success! BodyLength is %d", ServerTypeNames[to], nMsgId, s.size());
#endif
	return pNet->SendMsgWithOutHead(nMsgId, s.c_str(), s.size(), nSockIndex);
}

bool NetProxy::SendMsgAll(const MPMsg::MsgType nMsgType, const uint8_t from, const uint8_t to, const uint16_t nMsgId, const google::protobuf::Message& msg)
{
	auto pNet = GetNetModule(to);
	if (pNet == nullptr)
	{
		MP_ERROR("SendMsgAll Error!NetModule [%d] Not Found!", to);
		return false;
	}
	
	MPMsg::MsgBase msg_base;
	msg_base.set_msg_type(nMsgType);
	msg_base.set_msg_id(nMsgId);
	msg_base.set_msg_data(msg.SerializeAsString());
	auto s = msg_base.SerializeAsString();
	return pNet->SendMsgToAllWithOutHead(nMsgId, s.c_str(), s.size());
}

bool NetProxy::SendMsg(const uint8_t to, MPSOCK nSockIndex, const std::string& sData)
{
	auto pNet = GetNetModule(to);
	if (pNet == nullptr)
	{
		MP_ERROR("SendMsgAll Error!NetModule [%d] Not Found!", to);
		return false;
	}
	return pNet->SendMsg(sData.c_str(), (uint32_t)sData.size(), nSockIndex);
}

bool NetProxy::SendMsgAll(const uint8_t to, const std::string& sData)
{
	auto pNet = GetNetModule(to);
	if (pNet == nullptr)
	{
		MP_ERROR("SendMsgAll Error!NetModule [%d] Not Found!", to);
		return false;
	}
	pNet->SendMsgAll(sData.c_str(),(uint32_t)sData.size());
	return true;
}

void NetProxy::DelNetObject(const uint8_t nType,const MPSOCK nSockIndex)
{
	auto pNet = GetNetModule(nType);
	if (pNet == nullptr)
	{
		MP_ERROR("SendMsgAll Error!NetModule [%d] Not Found!", nType);
	}
	pNet->DelNetObject(nSockIndex);
}

void NetProxy::TryDump()
{
	int *p;
	p = (int*)0x100;
	*p = 0;
}

void NetProxy::CV_Wait()
{
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);
}

void NetProxy::CV_NotifyAll()
{
	cv.notify_all();
}

bool NetProxy::processUIDCB(const uint8_t nType, const MPGUID uid, const int nMsgID, MPMsg::MsgBase& msg)
{
	auto itType = m_mPBCallBacks.find(nType);
	if (itType == m_mPBCallBacks.end())
	{
		//MP_ERROR("OnRecieveMessage Type [%d] not found !", nType);
		return false;
	}

	auto itCB = itType->second.find(nMsgID);
	if (itCB == itType->second.end())
	{
		//MP_ERROR("OnRecieveMessage MsgId [%d] not found !", nMsgID);
		return false;
	}

	itCB->second->operator()(uid, msg.msg_data());
	return true;
}

bool NetProxy::processSOCKCB(const uint8_t nType, const MPSOCK nSockIndex, const int nMsgID, MPMsg::MsgBase& msg)
{
	auto itType = m_mSockCallBacks.find(nType);
	if (itType == m_mSockCallBacks.end())
	{
		//MP_ERROR("OnRecieveMessage Type [%d] not found !", nType);
		return false;
	}

	auto itCB = itType->second.find(nMsgID);
	if (itCB == itType->second.end())
	{
		//MP_ERROR("OnRecieveMessage MsgId [%d] not found !", nMsgID);
		return false;
	}

	itCB->second->operator()(msg.msg_data(),nSockIndex);
	return true;
}

bool NetProxy::processUIDCBEX(const uint8_t nType, const MPGUID uid, MPMsg::MsgBase& msg)
{
	auto itType = m_mExPBCallBacks.find(nType);
	if (itType == m_mExPBCallBacks.end())
	{
		return false;
	}
	for (auto pFunc : itType->second)
	{
		if (pFunc->operator()(uid,msg))
		{
			return true;
		}
	}
	return false;
}

bool NetProxy::processSOCKCBEX(const uint8_t nType, const MPSOCK nSockIndex, MPMsg::MsgBase& msg)
{
	auto itType = m_mExSockCallBacks.find(nType);
	if (itType == m_mExSockCallBacks.end())
	{
		return false;
	}
	for (auto pFunc : itType->second)
	{
		if (pFunc->operator()(msg, nSockIndex))
		{
			return true;
		}
	}
	return false;
}

void NetProxy::Kick(const uint8_t nType, const MPSOCK nSockIndex,const char * reason)
{
	auto pNetModule = GetNetModule(nType);
	auto pNetObject = pNetModule->GetNetObject(nSockIndex);
	if (pNetObject == nullptr)
	{
		return;
	}

	pNetModule->DelNetObject(nSockIndex);
	OnClientDisconnect(nType, nSockIndex);

	MP_INFO("Kick NetObject [%d][%llu] For Reason [%s]", nType, nSockIndex, reason);
}