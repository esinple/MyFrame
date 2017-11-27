#include "MPUDPServer.h"
#include "udp_server.h"
#include "udp_message.h"
#include "MPThread.h"
#include "buffer.h"
#include "event_loop.h"
#include "event_loop_thread_pool.h"

using namespace meplay;
using namespace evpp;

MPUDPServer::MPUDPServer(const std::string& sServerName, uint8_t nServerType, int nPort, uint8_t nThreadCount)
	: m_nPort(nPort),MPThread(sServerName + "_UdpService")
{
	//m_pUdpServer = std::make_shared<evpp::udp::Server>();

	SetServerType(nServerType);
	SetNetType(MP_NET_TYPE_UDP_SERVER);
}

MPUDPServer::~MPUDPServer()
{
	//while (!m_pTcpServer->IsStopped())
	//{
	//}
}

bool MPUDPServer::Execute()
{
	if (0)
	{
		Final();
	}
	return true;
}

void MPUDPServer::Run()
{
	m_pEventLoop->Run();
}

int MPUDPServer::InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)
{
	return -1;
}

void MessageCB(evpp::EventLoop* loop,evpp::udp::MessagePtr& msg)
{
}

int MPUDPServer::InitializationAsServer(const char* strIP,const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount)
{
	(void)nMaxClient;
	m_nThreadCount = nThreadCount;

	evpp::udp::Server server;
	m_pUdpServer->SetMessageHandler(MessageCB);
	m_pUdpServer->Init(m_nPort);
	m_pUdpServer->Start();

	m_pEventLoopPool = std::make_shared<evpp::EventLoopThreadPool>(m_pEventLoop.get(), m_nThreadCount);
	m_pEventLoopPool->Start(true);
	server.SetEventLoopThreadPool(m_pEventLoopPool);

	return 0;
}
bool MPUDPServer::Final()
{
	if (!m_pUdpServer->IsStopped())
	{
		m_pUdpServer->Stop(true);
		while (!m_pUdpServer->IsStopped())
		{
			this->MSleep(100);
		}
		m_pEventLoopPool->Stop(true);
		while (!m_pEventLoopPool->IsStopped())
		{
			this->MSleep(100);
		}
	}
	MPThread::ThreadFinal();
	return true;
}
