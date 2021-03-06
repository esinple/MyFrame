#include "MPTCPClient.h"
#include "tcp_client.h"
#include "event_loop.h"
#include "MPThread.h"
#include "tcp_conn.h"
#include "buffer.h"
#include "MPNetObject.h"

using namespace meplay;
using namespace evpp;

MPTCPClient::~MPTCPClient()
{
}

bool MPTCPClient::Execute()
{
	MPThread::ThreadStart();
	if (0)
	{
		Final();
	}
	
	return true;
}

void MPTCPClient::Run()
{
	MP_DEBUG("MPTCPClient Start Run!");
	m_pEventLoop->Run();
	MP_DEBUG("MPTCPClient End Run!");
	while (!IsThreadFinal())
	{
	}
	MP_DEBUG("MPTCPClient Run Over!");
}

void MPTCPClient::connectCB(const std::shared_ptr<evpp::TCPConn>& pConn)
{
	if (pConn->IsConnected())
	{
		//new connect
		auto pNetObj = std::make_shared<MPTCPObject>(pConn);
		AddNetObject(pConn->fd(), pNetObj);
		m_ConnectCB(GetServerType(), pConn->fd());
	}
	else
	{
		//disconnect
		m_DisConnectCB(GetServerType(), pConn->fd());
		DelNetObject(pConn->fd());

		cv.notify_all();
		//auto pNetObj = GetNetObject(pConn->fd());
		//if (pNetObj != nullptr)
		//{
		//	auto pClient = (MPTCPClientObj*)pNetObj.get();
		//	pClient->GetClient()->Disconnect();
		//	//DelNetObject(pConn->fd());
		//}
	}
}

void MPTCPClient::messageCB(const std::shared_ptr<evpp::TCPConn>& pConn, evpp::Buffer* pBuffer)
{
	auto pNetObj = GetNetObject(pConn->fd());
	if (pNetObj == nullptr)
	{
		return;
	}
	m_MsgCB(GetServerType(), pNetObj->GetRealFD(), pBuffer->data(), (uint32_t)pBuffer->length());
	pBuffer->Reset();
}

int MPTCPClient::InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)
{
#ifdef WIN_SYSTEM
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		MP_ERROR("WSAStartup failed with error: %d\n", err);
	}
#endif
	if (m_pEventLoop == nullptr)
	{
		m_pEventLoop.reset(new EventLoop());
		//m_pEventLoop = new EventLoop();
	}
	auto pClient = std::make_shared<TCPClient>(m_pEventLoop.get(), std::string(strIP) + ":" + std::to_string(nPort), m_sServiceName);
	//auto pClient = new TCPClient(m_pEventLoop, std::string(strIP) + ":" + std::to_string(nPort), m_sServiceName);

	pClient->SetConnectionCallback(std::bind(&MPTCPClient::connectCB, this, std::placeholders::_1));
	pClient->SetMessageCallback(std::bind(&MPTCPClient::messageCB, this, std::placeholders::_1, std::placeholders::_2));

	pClient->Connect();
	pClient->set_auto_reconnect(bAutoReconnect);
	pClient->set_reconnect_interval(Duration(milliseconds/1000.0f));

	m_vWaitClients.emplace_back(pClient);

	//auto pNetObject = std::make_shared<MPNetObject>(pClient);

	//m_mNetObjects.emplace(pNetObject->GetRealFD(), pNetObject);

	return 0;
}

int MPTCPClient::InitializationAsServer(const char* strIP, const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount)
{

	return -1;
}

bool MPTCPClient::Final()
{
	auto mNetObjs = GetAllNetObject();
	for (auto& ni : mNetObjs)
	{
		ni.second->Close();
		std::unique_lock<std::mutex> lck(mtx);
		cv.wait(lck);
	}

	//auto vWaitClients = m_vWaitClients;
	for (auto& pWaitObj : m_vWaitClients)
	{
		pWaitObj->set_auto_reconnect(false);
		pWaitObj->Disconnect();
		/*std::unique_lock<std::mutex> lck(mtx);
		cv.wait(lck);*/
	}

	if (!m_pEventLoop->IsStopped())
	{
		while (!m_pEventLoop->IsRunning())
		{
			this->MSleep(100);
		}

		m_pEventLoop->Stop();

		while (!m_pEventLoop->IsStopped())
		{
			this->MSleep(100);
		}
	}

	/*for (auto& pWaitObj : m_vWaitClients)
	{
		delete pWaitObj;
		pWaitObj = nullptr;
	}*/
	m_vWaitClients.clear();
	
	m_pEventLoop.reset();
	/*if(m_pEventLoop != nullptr)
	{
		delete m_pEventLoop;
		m_pEventLoop = nullptr;
	}*/

	MPThread::ThreadFinal();

#ifdef WIN_SYSTEM
	WSACleanup();
#endif
	return true;
}
