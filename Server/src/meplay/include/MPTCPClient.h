#pragma once
#include "MPNet.h"
#include "MPThread.h"
#include <list>

namespace evpp {
	class EventLoop;
	class TCPClient;
	class TCPConn;
	class Buffer;
}


namespace meplay {
	class MPNetObject;
	class MPTCPClient : public MPNet, public MPThread
	{
	public:
		template<typename BaseType>
		explicit MPTCPClient(
			const std::string& sClientName,
			uint8_t nServerType,
			BaseType* pBaseType,
			void (BaseType::*handleConnect)(const uint8_t, const MPSOCK),
			void (BaseType::*handleDisConnect)(const uint8_t, const MPSOCK),
			void (BaseType::*handleMsg)(const uint8_t, const MPSOCK, const char *, const uint32_t)
		) : MPThread(sClientName + "_TcpClient"), m_bStart(false),m_nPort(0), m_nThreadCount(0), m_sServiceName(sClientName + "_TcpClient"),m_pEventLoop(nullptr)
		{
			m_ConnectCB = std::bind(handleConnect, pBaseType, std::placeholders::_1, std::placeholders::_2);
			m_DisConnectCB = std::bind(handleDisConnect, pBaseType, std::placeholders::_1, std::placeholders::_2);
			m_MsgCB = std::bind(handleMsg, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

			SetNetType(MP_NET_TYPE_TCP_CLIENT);
			SetServerType(nServerType);
		};
		//MPTCPClient(, uint8_t nServerType, int nPort, uint8_t nThreadCount);
		virtual ~MPTCPClient();
	public:
		virtual bool Execute()override;

		virtual int InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)override;

		virtual int InitializationAsServer(const char* strIP, const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount = 4)override;

		virtual bool Final()override;

		virtual void Run()override;
	private:
		void connectCB(const std::shared_ptr<evpp::TCPConn>& pConn);
		void messageCB(const std::shared_ptr<evpp::TCPConn>& pConn, evpp::Buffer* pBuffer);
	private:
		bool m_bStart;
		int m_nPort;
		int m_nThreadCount;
		const std::string m_sServiceName;
		std::shared_ptr<evpp::EventLoop> m_pEventLoop;
		//evpp::EventLoop* m_pEventLoop;
		std::list<std::shared_ptr<evpp::TCPClient>> m_vWaitClients;
		//std::list<evpp::TCPClient*> m_vWaitClients;
		//std::unordered_map<MPSOCK,std::shared_ptr<evpp::TCPClient>> m_pTcpClients;

		NET_CONNECT_FUNCTOR m_ConnectCB;
		NET_DISCONNECT_FUNCTOR m_DisConnectCB;
		NET_TYPE_DATA_FUNCTOR m_MsgCB;
	};
}