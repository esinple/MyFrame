#pragma once
#include "MPNet.h"
#include "MPThread.h"

namespace evpp {
	class EventLoop;
	class TCPServer;
	class TCPConn;
	class Buffer;
}

namespace meplay{
	class MPNetObject;
	class MPTCPServer : public MPNet ,public MPThread
	{
	public:
		template<typename BaseType>
		explicit MPTCPServer(
			const std::string& sServerName,
			uint8_t nServerType,
			BaseType* pBaseType,
			void (BaseType::*handleConnect)(const uint8_t, const MPSOCK),
			void (BaseType::*handleDisConnect)(const uint8_t, const MPSOCK),
			void (BaseType::*handleMsg)(const uint8_t, const MPSOCK, const char * ,const uint32_t)
		) : MPThread(sServerName + "_TcpService"), m_nPort(0), m_nThreadCount(0), m_sServiceName(sServerName + "_TcpService")
		{
			m_ConnectCB = std::bind(handleConnect, pBaseType, std::placeholders::_1, std::placeholders::_2);
			m_DisConnectCB = std::bind(handleDisConnect, pBaseType, std::placeholders::_1, std::placeholders::_2);
			m_MsgCB = std::bind(handleMsg, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

			SetNetType(MP_NET_TYPE_TCP_SERVER);
			SetServerType(nServerType);
		};
		//MPTCPServer(, uint8_t nServerType, int nPort, uint8_t nThreadCount);
		virtual ~MPTCPServer();
	public:
		virtual bool Execute()override;

		virtual int InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)override;

		virtual int InitializationAsServer(const char* strIP,const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount = 4)override;

		virtual bool Final()override;

		virtual void Run()override;
	private:
		void connectCB(const std::shared_ptr<evpp::TCPConn>& pConn);
		void messageCB(const std::shared_ptr<evpp::TCPConn>& pConn, evpp::Buffer* pBuffer);
	private:
		int m_nPort;
		int m_nThreadCount;
		const std::string m_sServiceName;
		std::shared_ptr<evpp::EventLoop> m_pEventLoop;
		std::shared_ptr<evpp::TCPServer> m_pTcpServer;
		std::unordered_map<MPSOCK, MPNetObjectPtr> m_mNetObjects;

		NET_CONNECT_FUNCTOR m_ConnectCB;
		NET_DISCONNECT_FUNCTOR m_DisConnectCB;
		NET_TYPE_DATA_FUNCTOR m_MsgCB;
	};
}