#include "NetProxy.h"
#include "MPLogger.h"
#include "termcolor.hpp"
#include "MPMsgBase.pb.h"
#include "MPTCPServer.h"
#include "MPTCPClient.h"
#include "MPRUDPServer.h"
#include "MPRUDPClient.h"
#include "CommDef.h"
#ifdef SSJ_DEBUG
#include "MPTimeTester.h"
#endif

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
#ifdef SSJ_DEBUG
	meplay::MPTimeTester tester("NetLoopTester", 200);
#endif
	asyncRead();
#ifdef SSJ_DEBUG
	tester.Show(200);
#endif
	asyncWrite();
#ifdef SSJ_DEBUG
	tester.Show(200);
#endif
	asyncConn();
#ifdef SSJ_DEBUG
	tester.Show(200);
#endif
	asyncDisconn();
#ifdef SSJ_DEBUG
	tester.Show(200);
#endif
	tickNetModule();
#ifdef SSJ_DEBUG
	tester.Show(200);
#endif
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
	if (m_bFinal)
	{
		return;
	}
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

bool NetProxy::AddTCPServerModule(uint8_t nType, const uint32_t nMaxClient, const char* ip, const uint16_t nPort, const uint8_t nThreadCount)
{
	auto pNet = GetNetModule(nType);
	if (pNet == nullptr)
	{
		pNet = std::shared_ptr<MPTCPServer>(new MPTCPServer(
			"NetProxy", 
			nType, 
			this,
			&NetProxy::OnConnCB, 
			&NetProxy::OnDisConnCB,
			&NetProxy::OnMsgCB
		));

		if (pNet->InitializationAsServer(ip, nMaxClient, nPort, nThreadCount) < 0)
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
		pNet = std::shared_ptr<MPTCPClient>(new MPTCPClient(
			"NetProxy", 
			nType, 
			this, 
			&NetProxy::OnConnCB,
			&NetProxy::OnDisConnCB, 
			&NetProxy::OnMsgCB
		));
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
	//auto pNet = GetNetModule(nType);
	//if (pNet == nullptr)
	//{

	//	pNet = std::shared_ptr<MPUDPServer>(new MPUDPServer(
	//		"NetProxy", 
	//		nType, 
	//		this, 
	//		&NetProxy::OnConnCB, 
	//		&NetProxy::OnDisConnCB, 
	//		&NetProxy::OnMsgCB
	//	));
	//	if (pNet->InitializationAsServer(0/*nMaxClient*/, nPort, nThreadCount) < 0)
	//	{
	//		MP_ERROR("Init ServerNetModule Error![%d][MaxClient : %d][Port : %d]", nType, 0/*nMaxClient*/, nPort);
	//		ImmediatelyFinal("Init Server Net Faild!");
	//		return false;
	//	}
	//	MP_SYSTEM("Init ServerNetModule Success![%d][MaxClient : %d][Port : %d]", nType, 0/*nMaxClient*/, nPort);
	//	m_mNetModules.emplace(nType, pNet);
	//}
	//else
	//{
	//	MP_ERROR("Already Has ServerNetModule ![%d][MaxClient : %d][Port : %d]", nType, 0/*nMaxClient*/, nPort);
	//	ImmediatelyFinal("Init Server Net Faild!");
	//	return false;
	//}
	return true;
}

bool NetProxy::AddRUDPServerModule(
	uint8_t nType,
	const uint32_t nMaxClient,
	const char* ip,
	const uint16_t nPort,
	const uint8_t nThreadCount
)
{
	auto pNet = GetNetModule(nType);
	if (pNet == nullptr)
	{

		pNet = std::shared_ptr<MPRUDPServer>(new MPRUDPServer(
			"NetProxy",
			nType,
			this,
			&NetProxy::OnConnCB,
			&NetProxy::OnDisConnCB,
			&NetProxy::OnMsgCB
		));
		if (pNet->InitializationAsServer(ip, nMaxClient, nPort, nThreadCount) < 0)
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

void NetProxy::AddRUDPClientModule(uint8_t nType, const char* ip, const uint16_t nPort, bool bAutoConnect)
{
	auto pNet = GetNetModule(nType);
	if (pNet == nullptr)
	{
		pNet = std::shared_ptr<MPRUDPClient>(new MPRUDPClient(
			"NetProxy",
			nType,
			this,
			&NetProxy::OnConnCB,
			&NetProxy::OnDisConnCB,
			&NetProxy::OnMsgCB
		));
		m_mNetModules.emplace(nType, pNet);
	}

	if (pNet->InitializationAsClient(ip, nPort, bAutoConnect, 3000) < 0)
	{
		MP_ERROR("Init ClientNetModule Error![%d][IP : %s][Port : %d]", nType, ip, nPort);
		return;
	}

	MP_SYSTEM("Init ClientNetModule Success![%d][IP: %s][Port : %d]", nType, ip, nPort);
}

void NetProxy::InvalidMessage(const uint8_t nType, const MPSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
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
	int64_t nOffLen(nLen);

	while (nOffLen > MPMsgHead::MP_Head::MP_HEAD_LENGTH)
	{
		MPMsgHead xHead;
		int nMsgBodyLength = pNet->DeCode(pOffset, nLen, xHead);
		if (nMsgBodyLength > 0 && nMsgBodyLength < nOffLen && xHead.GetMsgID() > 0)
		{
			//OnRecieveMessage(nType,pNetObject->GetRealFD(), xHead.GetMsgID(), pOffset + MPMsgHead::MP_Head::MP_HEAD_LENGTH, nMsgBodyLength);
			m_Buffers.AddRecvMsg(
				nType, 
				pNetObject->GetRealFD(), 
				xHead.GetMsgID(), 
				pOffset + MPMsgHead::MP_Head::MP_HEAD_LENGTH, 
				nMsgBodyLength
			);

			nOffLen -= nMsgBodyLength + MPMsgHead::MP_Head::MP_HEAD_LENGTH;
			pOffset += nMsgBodyLength + MPMsgHead::MP_Head::MP_HEAD_LENGTH;
#ifdef SSJ_DEBUG
			MP_DEBUG("[%s] Recieve Msg [%d] Success! BodyLength is %d",
				GetServerTypeNames(nType), xHead.GetMsgID(), nMsgBodyLength);
#endif
		}
		else
		{
#ifdef SSJ_DEBUG
			MP_DEBUG("[%s] Recieve Msg Failed! BodyLength is %d", GetServerTypeNames(nType), nMsgBodyLength);
#endif
		}
	}
}

void NetProxy::OnConnCB(const uint8_t nType, const MPSOCK nSockIndex)
{
	m_vConnectList.emplace_back(std::make_tuple(nType, nSockIndex));
}

void NetProxy::OnDisConnCB(const uint8_t nType, const MPSOCK nSockIndex)
{
	m_vDisConnectList.emplace_back(std::make_tuple(nType, nSockIndex));
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

bool NetProxy::SendMsg(
	const MPMsg::MsgType nMsgType,
	const uint8_t from,
	const uint8_t to, 
	const MPSOCK nSockIndex, 
	const uint16_t nMsgId,
	const google::protobuf::Message& msg
)
{
	MPMsg::MsgBase msg_base;
	msg_base.set_msg_type(nMsgType);
	msg_base.set_msg_id(nMsgId);
	msg_base.set_msg_data(msg.SerializeAsString());
	auto s = msg_base.SerializeAsString();
#ifdef SSJ_DEBUG
	MP_DEBUG("[%s] Send Msg [%d] Success! BodyLength is %d", GetServerTypeNames(to), nMsgId, s.size());
#endif
	m_Buffers.AddSendMsg(
		to,
		std::list<MPSOCK>{nSockIndex},
		nMsgId,
		s.c_str(),
		(uint32_t)s.size()
	);
	return true;
}

bool NetProxy::SendMsg(
	const MPMsg::MsgType nMsgType,
	const uint8_t from,
	const uint8_t to,
	std::list<MPSOCK> vSockIndex,
	const uint16_t nMsgId,
	const google::protobuf::Message& msg
)
{
	MPMsg::MsgBase msg_base;
	msg_base.set_msg_type(nMsgType);
	msg_base.set_msg_id(nMsgId);
	msg_base.set_msg_data(msg.SerializeAsString());
	auto s = msg_base.SerializeAsString();
#ifdef SSJ_DEBUG
	MP_DEBUG("[%s] Send Msg [%d] Success! BodyLength is %d", GetServerTypeNames(to), nMsgId, s.size());
#endif
	m_Buffers.AddSendMsg(
		to,
		std::move(vSockIndex),
		nMsgId,
		s.c_str(),
		(uint32_t)s.size()
	);
	return true;
}

bool NetProxy::SendMsgAll(const MPMsg::MsgType nMsgType, const uint8_t from, const uint8_t to, const uint16_t nMsgId, const google::protobuf::Message& msg)
{
	MPMsg::MsgBase msg_base;
	msg_base.set_msg_type(nMsgType);
	msg_base.set_msg_id(nMsgId);
	msg_base.set_msg_data(msg.SerializeAsString());
	auto s = msg_base.SerializeAsString();
	m_Buffers.AddSendMsg(
		to,
		std::list<MPSOCK>(),
		nMsgId,
		s.c_str(),
		(uint32_t)s.size()
	);
	return true;
}

bool NetProxy::SendMsgWithHead(const uint8_t to, MPSOCK nSockIndex, const std::string& sData)
{
	auto pNet = GetNetModule(to);
	if (pNet == nullptr)
	{
		MP_ERROR("SendMsgAll Error!NetModule [%d] Not Found!", to);
		return false;
	}
	return pNet->SendMsg(sData.c_str(), (uint32_t)sData.size(), nSockIndex);
}

bool NetProxy::SendMsgAllWithHead(const uint8_t to, const std::string& sData)
{
	auto pNet = GetNetModule(to);
	if (pNet == nullptr)
	{
		MP_ERROR("SendMsgAll Error!NetModule [%d] Not Found!", to);
		return false;
	}
	pNet->SendMsgAll(sData.c_str(), (uint32_t)sData.size());
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

void NetProxy::asyncRead()
{
	//Real Recv
	auto pRecvMsg = m_Buffers.GetRecvMsg();
	while (pRecvMsg != nullptr)
	{
		for (auto& nSockIndex : pRecvMsg->m_vSockIndex)
		{
			OnRecieveMessage(
				pRecvMsg->m_nNetType,
				nSockIndex,
				pRecvMsg->m_nMsgId,
				(const char *)(&pRecvMsg->m_vData[0]),
				(uint32_t)pRecvMsg->m_vData.size()
			);
		}
		pRecvMsg = m_Buffers.GetRecvMsg();
	}

}

void NetProxy::asyncWrite()
{
	//Real Send
	auto pSendMsg = m_Buffers.GetSendMsg();
	while (pSendMsg != nullptr)
	{
		auto pNet = GetNetModule(pSendMsg->m_nNetType);
		if (pNet == nullptr)
		{
			MP_ERROR("SendMsgAll Error!NetModule [%d] Not Found!", pSendMsg->m_nNetType);
			pSendMsg = m_Buffers.GetSendMsg();
			continue;
		}

		if (pSendMsg->m_vSockIndex.empty())
		{
			//broadcast
			pNet->SendMsgToAllWithOutHead(
				pSendMsg->m_nMsgId,
				(const char *)(&pSendMsg->m_vData[0]),
				(uint32_t)pSendMsg->m_vData.size()
			);
		}
		else
		{
			pNet->SendMsgWithOutHead(
				pSendMsg->m_nMsgId,
				(const char *)(&pSendMsg->m_vData[0]),
				(uint32_t)pSendMsg->m_vData.size(),
				pSendMsg->m_vSockIndex
			);
		}
		pSendMsg = m_Buffers.GetSendMsg();
	}
}

void NetProxy::asyncConn()
{
	std::unique_lock<std::mutex> lck(mtx);
	auto vConnList = m_vConnectList;
	m_vConnectList.clear();
	lck.unlock();

	for (auto&&[nType, nSockIndex] : vConnList)
	{
		OnClientConnected(nType, nSockIndex);
	}
}

void NetProxy::asyncDisconn()
{
	std::unique_lock<std::mutex> lck(mtx);
	auto vDisConnList = m_vDisConnectList;
	m_vDisConnectList.clear();
	lck.unlock();

	for (auto&&[nType, nSockIndex] : vDisConnList)
	{
		OnClientDisconnect(nType, nSockIndex);
	}
}

void NetProxy::tickNetModule()
{
	for (auto& clients : m_mNetModules)
	{
		auto pNetModule = clients.second;
		pNetModule->Execute();
	}
}
