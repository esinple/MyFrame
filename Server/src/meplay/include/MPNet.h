// -------------------------------------------------------------------------
//    @FileName			:		MPINet.h
//    @Author			:		LvSheng.Huang
//    @Date				:		2012-12-15
//    @Module			:		MPINet
// -------------------------------------------------------------------------

#ifndef MP_NET_BASE_H
#define MP_NET_BASE_H

#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include "MPGUID.h"
#include "MPNetDefine.h"
#include "MPNetObject.h"
#include "MPMsg.h"

#ifdef WIN_SYSTEM
#include <WinSock2.h>
#else
//#include <libkern/OSByteOrder.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#pragma pack(push, 1)

namespace meplay {
	typedef std::function<void(const uint8_t nType, const MPSOCK nSockIndex)> NET_CONNECT_FUNCTOR;
	typedef std::shared_ptr<NET_CONNECT_FUNCTOR> NET_CONNECT_FUNCTOR_PTR;
	typedef std::function<void(const uint8_t nType, const MPSOCK nSockIndex)> NET_DISCONNECT_FUNCTOR;
	typedef std::shared_ptr<NET_DISCONNECT_FUNCTOR> NET_DISCONNECT_FUNCTOR_PTR;
	typedef std::function<void(const uint8_t nType, const MPSOCK nSockIndex, const char* msg, const uint32_t nLen)> NET_TYPE_DATA_FUNCTOR;
	typedef std::shared_ptr<NET_TYPE_DATA_FUNCTOR> NET_TYPE_DATA_FUNCTOR_PTR;

	class MPNetBase
	{
	public:
		MPNetBase() {};
		virtual ~MPNetBase() {};

		//need to call this function every frame to drive network library
		virtual bool Execute() = 0;

		virtual int InitializationAsClient(const char* strIP, const unsigned short nPort,bool bAutoReconnect,const uint32_t milliseconds) = 0;

		virtual int InitializationAsServer(const char* strIP, const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;

		virtual int ExpandBufferSize(const unsigned int size) { return 0; };

		virtual bool Final() = 0;

		//send a message with out msg-head[auto add msg-head in this function]
		virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const MPSOCK nSockIndex = 0) = 0;

		//send a message with out msg-head to all client[auto add msg-head in this function]
		virtual bool SendMsgToAllWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen) = 0;
	};

	class MPNet : public MPNetBase
	{
	public:
		MPNet();
		virtual ~MPNet();
	public:
		//send a message with out msg-head[auto add msg-head in this function]
		virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const MPSOCK nSockIndex)override;

		//send a message with out msg-head to all client[auto add msg-head in this function]
		virtual bool SendMsgToAllWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen)override;

		virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const std::list<MPSOCK>& fdList);
		
	public:
		std::map<MPSOCK, MPNetObjectPtr>& GetAllNetObject();

		MPNetObjectPtr GetNetObject(const MPSOCK nSockIndex);

		bool AddNetObject(const MPSOCK nSockIndex, MPNetObjectPtr pObject);

		bool DelNetObject(const MPSOCK nSockIndex);

		void Clear();

		MP_NET_TYPE GetNetType()const;
		void SetNetType(MP_NET_TYPE nType);

		uint8_t GetServerType()const;
		void SetServerType(uint8_t nType);

		bool SendMsg(const char* msg, const uint32_t nLen, const MPSOCK nSockIndex);
		bool SendMsg(const char* msg, const uint32_t nLen, const std::list<MPSOCK>& nSockIndex);
		void SendMsgAll(const char* msg, const uint32_t nLen);

		static int DeCode(const char* strData, const uint32_t unLen, MPMsgHead& xHead);
		static int EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData);

		void KeepAlive();

	private:
		std::map<MPSOCK, MPNetObjectPtr> m_mNetObjects;
		MP_NET_TYPE m_nType;
		uint8_t m_nServerType;
	};
}

#pragma pack(pop)

#endif
