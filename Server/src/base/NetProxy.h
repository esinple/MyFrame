#pragma once

#include <array>
#include <vector>
#include <atomic>

#include "MPThread.h"
#include "MPModule.h"
#include "MPTime.h"
#include "MPGUID.h"
#include "MPNetDefine.h"
#include <google/protobuf/message.h>
#include "MPLogger.h"
#include "MPMsg.h"
#include "MPMsgBase.pb.h"
#include "MPNet.h"

typedef std::function<void(const MPGUID,const std::string&)> PB_RECEIVE_FUNCTOR;
typedef std::shared_ptr<PB_RECEIVE_FUNCTOR> PB_RECEIVE_FUNCTOR_PTR;

typedef std::function<void(const std::string&,const uint64_t)> SOCK_RECEIVE_FUNCTOR;
typedef std::shared_ptr<SOCK_RECEIVE_FUNCTOR> SOCK_RECEIVE_FUNCTOR_PTR;

typedef std::function<bool(MPMsg::MsgBase& msg, const uint64_t)> EX_SOCK_RECEIVE_FUNCTOR;
typedef std::shared_ptr<EX_SOCK_RECEIVE_FUNCTOR> EX_SOCK_RECEIVE_FUNCTOR_PTR;

typedef std::function<bool(const MPGUID uid,MPMsg::MsgBase& msg)> EX_PB_RECEIVE_FUNCTOR;
typedef std::shared_ptr<EX_PB_RECEIVE_FUNCTOR> EX_PB_RECEIVE_FUNCTOR_PTR;

using namespace meplay;

class NetProxy
{
public:
	NetProxy(const time_t nGapMilliSeconds);
	virtual ~NetProxy();
public:
	virtual bool InitServerCfg(const std::string& filename) = 0;
	virtual void OnClientDisconnect(const uint8_t nType, const MPSOCK nSockIndex) = 0;
	virtual void OnClientConnected(const uint8_t nType,const MPSOCK nSockIndex) = 0;
	virtual void LogicStart() = 0;
	virtual void LogicFinal() = 0;
	virtual void LogicRun() = 0;
	virtual const MPGUID GetGUIDBySock(const uint8_t nType,const MPSOCK nSockIndex)const = 0;
public:
	bool AddTCPServerModule(uint8_t nType, const uint32_t nMaxClient, const char* ip, const uint16_t nPort, const uint8_t nThreadCount = 4);
	void AddTCPClientModule(uint8_t nType, const char* ip, const uint16_t nPort,bool bAutoConnect = true);
	bool AddUDPServerModule(uint8_t nType, const uint16_t nPort,const uint8_t nThreadCount = 4);

	void Start();
	void Final(const char* reason);
	void ImmediatelyFinal(const char* reason);
	void NetRun();
	void NetFinal();

	bool IsFinal()const;

	void Kick(const uint8_t nType, const MPSOCK nSockIndex,const char* reason = "Unknown");

	void DelNetObject(const uint8_t nType,const MPSOCK nSockIndex);

	void OnRecieveMessage(const uint8_t nType,const MPSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen);
	void InvalidMessage(const uint8_t nType,const MPSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen);

	void OnMsgCB(const uint8_t nType,const MPSOCK nSockIndex, const char * msg, const uint32_t nLen);

	std::shared_ptr<MPNet> GetNetModule(uint32_t nType);

	template<typename BaseType>
	bool AddReceiveCallBack(const uint32_t nType, BaseType* pBase, const uint16_t nMsgId, void(BaseType::*handleRecieve)(const MPGUID uid, const std::string&))
	{
		auto f = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		PB_RECEIVE_FUNCTOR_PTR pFunc(new PB_RECEIVE_FUNCTOR(f));
		return m_mPBCallBacks[nType].emplace(nMsgId, pFunc).second;
	}

	template<typename BaseType>
	bool AddReceiveCallBack(const uint32_t nType, BaseType* pBase, const uint16_t nMsgId, void(BaseType::*handleRecieve)( const std::string&,const uint64_t nSockIndex))
	{
		auto f = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		SOCK_RECEIVE_FUNCTOR_PTR pFunc(new SOCK_RECEIVE_FUNCTOR(f));
		return m_mSockCallBacks[nType].emplace(nMsgId, pFunc).second;
	}

	template<typename BaseType>
	bool AddReceiveCallBackEx(const uint32_t nType, BaseType* pBase, bool(BaseType::*handleRecieve)(MPMsg::MsgBase&, const uint64_t nSockIndex))
	{
		auto f = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		EX_SOCK_RECEIVE_FUNCTOR_PTR pFunc(new EX_SOCK_RECEIVE_FUNCTOR(f));
		m_mExSockCallBacks[nType].emplace_back(pFunc);
		return true;
	}

	template<typename BaseType>
	bool AddReceiveCallBackEx(const uint32_t nType, BaseType* pBase, bool(BaseType::*handleRecieve)(const MPGUID uid,MPMsg::MsgBase&))
	{
		auto f = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		EX_PB_RECEIVE_FUNCTOR_PTR pFunc(new EX_PB_RECEIVE_FUNCTOR(f));
		m_mExPBCallBacks[nType].emplace_back(pFunc);
		return true;
	}

	bool SendMsg(const MPMsg::MsgType nMsgType,const uint8_t from,const uint8_t to,MPSOCK nSockIndex,const uint16_t nMsgId,const google::protobuf::Message& msg);
	bool SendMsgAll(const MPMsg::MsgType nMsgType,const uint8_t from,const uint8_t to, const uint16_t nMsgId,const google::protobuf::Message& msg);

	bool SendMsg(const uint8_t to, MPSOCK nSockIndex, const std::string& sData);
	bool SendMsgAll(const uint8_t to, const std::string& sData);

	void TryDump();

	void CV_Wait();
	void CV_NotifyAll();

private:
	bool processUIDCB(const uint8_t nType, const MPGUID uid, const int nMsgID, MPMsg::MsgBase& msg);
	bool processSOCKCB(const uint8_t nType, const MPSOCK nSockIndex, const int nMsgID, MPMsg::MsgBase& msg);
	bool processUIDCBEX(const uint8_t nType, const MPGUID uid, MPMsg::MsgBase& msg);
	bool processSOCKCBEX(const uint8_t nType, const MPSOCK nSockIndex, MPMsg::MsgBase& msg);
private:
	std::map<uint8_t, std::shared_ptr<MPNet>> m_mNetModules;

	std::map<uint8_t, std::unordered_map<uint16_t, PB_RECEIVE_FUNCTOR_PTR>> m_mPBCallBacks;
	std::map<uint8_t, std::unordered_map<uint16_t, SOCK_RECEIVE_FUNCTOR_PTR>> m_mSockCallBacks;
	std::map<uint8_t, std::list<EX_SOCK_RECEIVE_FUNCTOR_PTR>> m_mExSockCallBacks;
	std::map<uint8_t, std::list<EX_PB_RECEIVE_FUNCTOR_PTR>> m_mExPBCallBacks;

	std::atomic<bool> m_bFinal;

	meplay::MPTime m_tLastTime;

	const time_t m_nGapMilliSeconds;
protected:
	std::condition_variable cv;
	std::mutex mtx;
};