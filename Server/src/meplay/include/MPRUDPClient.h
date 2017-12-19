#pragma once

#pragma once
#include "MPNet.h"
#include "MPThread.h"
#include "enet/enet.h"

namespace meplay
{
	struct RUDPConnectData
	{
		ENetHost* m_pENetClient;
		ENetAddress m_ENetAddress;
		int m_nPort;
		bool m_bAutoConnect;
		uint32_t m_nTimeOut;
	};

	class MPRUDPClient : public MPNet, public MPThread
	{
	public:
		template<typename BaseType>
		explicit MPRUDPClient(
			const std::string& sServerName,
			uint8_t nServerType,
			BaseType* pBaseType,
			void (BaseType::*handleConnect)(const uint8_t, const MPSOCK),
			void (BaseType::*handleDisConnect)(const uint8_t, const MPSOCK),
			void (BaseType::*handleMsg)(const uint8_t, const MPSOCK, const char *, const uint32_t)
		) : MPThread(sServerName + "_RUDPService")
		{
			m_ConnectCB = std::bind(handleConnect, pBaseType, std::placeholders::_1, std::placeholders::_2);
			m_DisConnectCB = std::bind(handleDisConnect, pBaseType, std::placeholders::_1, std::placeholders::_2);
			m_MsgCB = std::bind(handleMsg, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

			SetNetType(MP_NET_TYPE_UDP_SERVER);
			SetServerType(nServerType);
		};
		virtual ~MPRUDPClient();
	public:

		virtual bool Execute()override;

		virtual int InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)override;

		virtual int InitializationAsServer(const char * strIP, const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount = 4)override;

		virtual bool Final()override;

		//send a message with out msg-head[auto add msg-head in this function]
		virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const MPSOCK nSockIndex)override;

		//send a message with out msg-head to all client[auto add msg-head in this function]
		virtual bool SendMsgToAllWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen)override;

		virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const std::list<MPSOCK>& fdList);

		virtual void Run()override;
	private:
		void tryConnect(RUDPConnectData& data);
		void connectCB(ENetEvent* pEvent);
		void disConnectCB(ENetEvent* pEvent);
		void messageCB(ENetEvent* pEvent);

		ENetPacket* createPacket(const int16_t nMsgID, const char* msg, const uint32_t nLen);

	private:
		std::list<RUDPConnectData> m_vConnects;
		
		NET_CONNECT_FUNCTOR m_ConnectCB;
		NET_DISCONNECT_FUNCTOR m_DisConnectCB;
		NET_TYPE_DATA_FUNCTOR m_MsgCB;
	};
}
