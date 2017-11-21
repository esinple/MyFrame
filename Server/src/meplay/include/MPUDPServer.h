#pragma once
#include "MPNet.h"
#include "MPThread.h"

namespace evpp{
	namespace udp {
		class Server;
	}
	class EventLoop;
	class EventLoopThreadPool;
}

namespace meplay {
	class MPUDPServer : public MPNet, public MPThread
	{
	public:
		MPUDPServer(const std::string& sServerName, uint8_t nServerType, int nPort, uint8_t nThreadCount);
		virtual ~MPUDPServer();
	public:

		virtual bool Execute()override;

		virtual int InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)override;

		virtual int InitializationAsServer(const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount = 4)override;

		virtual bool Final()override;

		virtual void Run()override;

	private:
		int m_nPort;
		int m_nThreadCount;
		std::shared_ptr<evpp::udp::Server> m_pUdpServer;
		std::shared_ptr<evpp::EventLoop> m_pEventLoop;
		std::shared_ptr<evpp::EventLoopThreadPool> m_pEventLoopPool;
		NET_TYPE_RECEIVE_FUNCTOR m_RecvCB;
		NET_TYPE_EVENT_FUNCTOR m_EventCB;

	};
}