#include "MPTCPServer.h"
#include "tcp_server.h"
#include "event_loop.h"
#include "MPThread.h"
#include "tcp_conn.h"
#include "buffer.h"
#include "MPNetObject.h"

using namespace meplay;
using namespace evpp;

//MPTCPServer::MPTCPServer(const std::string& sServerName, uint8_t nServerType, int nPort, uint8_t nThreadCount)
//	: MPThread(sServerName + "_TcpService")
//	, m_nPort(nPort)
//	, m_nThreadCount(nThreadCount)
//{
//	m_pEventLoop = std::make_shared<EventLoop>();
//	m_pTcpServer = std::make_shared<TCPServer>(m_pEventLoop.get(), "127.0.0.1:" + std::to_string(nPort), sServerName, m_nThreadCount);
//	
//	SetServerType(nServerType);
//	SetNetType(MP_NET_TYPE_TCP_SERVER);
//}

MPTCPServer::~MPTCPServer()
{
	//while (!m_pTcpServer->IsStopped())
	//{
	//}
}

bool MPTCPServer::Execute()
{
	MPThread::ThreadStart();
	if (0)
	{
		Final();
	}
	return true;
}

void MPTCPServer::Run()
{
	m_pEventLoop->Run();
	while(!IsThreadFinal())
	{
	}
}

int MPTCPServer::InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)
{
	return -1;
}

void MPTCPServer::connectCB(const std::shared_ptr<evpp::TCPConn>& pConn)
{
	if (pConn->IsConnected())
	{
		//new connect
		auto pNetObj = std::make_shared<MPNetObject>(pConn);
		AddNetObject(pConn->fd(), pNetObj);
		m_ConnectCB(GetServerType(), pConn->fd());
	}
	else
	{
		//disconnect
		m_DisConnectCB(GetServerType(), pConn->fd());
		DelNetObject(pConn->fd());

		cv.notify_all();
	}
	
}

void MPTCPServer::messageCB(const std::shared_ptr<evpp::TCPConn>& pConn, evpp::Buffer* pBuffer)
{
	auto pNetObj = GetNetObject(pConn->fd());
	if (pNetObj == nullptr)
	{
		return;
	}
	m_MsgCB(GetServerType(), pNetObj->GetRealFD(), pBuffer->data(), (uint32_t)pBuffer->length());
	pBuffer->Reset();
}

int MPTCPServer::InitializationAsServer(const char * strIP, const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount)
{
	(void)nMaxClient;
	m_nThreadCount = nThreadCount;
#ifdef WIN_SYSTEM
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		MP_ERROR("WSAStartup failed with error: %d\n", err);
		return -1;
	}
#endif
	m_pEventLoop = std::make_shared<EventLoop>();
	m_pTcpServer = std::make_shared<TCPServer>(m_pEventLoop.get(), std::string(strIP) + ":" + std::to_string(nPort), m_sServiceName, m_nThreadCount);

	m_pTcpServer->SetConnectionCallback(std::bind(&MPTCPServer::connectCB, this, std::placeholders::_1));
	m_pTcpServer->SetMessageCallback(std::bind(&MPTCPServer::messageCB, this, std::placeholders::_1, std::placeholders::_2));

	if (!m_pTcpServer->Init())
	{
		return -1;
	}

	if (!m_pTcpServer->Start())
	{
		return -1;
	}

	return 0;
}

bool MPTCPServer::Final()
{
//#ifdef WIN_SYSTEM
	MP_DEBUG("TcpServer Final!");
	auto mNetObjs = GetAllNetObject();
	for (auto& ni : mNetObjs)
	{
		ni.second->GetConn()->Close();
		std::unique_lock<std::mutex> lck(mtx);
		cv.wait(lck);
	}
	MP_DEBUG("TcpServer Close Conns!");

	if (!m_pTcpServer->IsStopped())
	{
		while (!m_pTcpServer->IsRunning())
		{
			this->MSleep(100);
		}
		m_pTcpServer->Stop();

		while (!m_pTcpServer->IsStopped())
		{
			MP_DEBUG("TcpServer Stoping!");
			this->MSleep(100);
		}
		MP_DEBUG("TcpServer Stopped!");
	}
//#endif

#ifdef WIN_SYSTEM
	WSACleanup();
#endif

	MPThread::ThreadFinal();
	return true;
}
