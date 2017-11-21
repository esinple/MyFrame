#include "MPNetObject.h"
#include "MPNet.h"
#ifdef WIN_SYSTEM
#else
#include <arpa/inet.h>
#endif

#include "tcp_conn.h"
#include "tcp_callbacks.h"
#include "tcp_client.h"

using namespace meplay;
using namespace evpp;

MPNetObject::MPNetObject(const std::shared_ptr<evpp::TCPConn>& pConn)
	: m_pConn(pConn)
{
	if (pConn == nullptr)
	{

	}
	else
	{
		m_nFD = m_pConn->fd();
		auto& sInfo = m_pConn->remote_addr();
		auto nPos = sInfo.find_first_of(':');
		m_sIP = sInfo.substr(0, nPos);
		m_nPort = std::atoi(sInfo.substr(nPos + 1, sInfo.size()).c_str());
	}
}

MPNetObject::~MPNetObject()
{
}

MPSOCK MPNetObject::GetRealFD()const
{
	return m_nFD;
}

const std::string& MPNetObject::GetIP()const
{
	return m_sIP;
}

const int MPNetObject::GetPort()const
{
	return m_nPort;
}

std::shared_ptr<evpp::TCPConn>& MPNetObject::GetConn()
{
	return m_pConn;
}

void MPNetObject::Send(const char * msg, const uint32_t len)
{
	m_pConn->Send(msg, len);
}

/*MPTCPClientObj::MPTCPClientObj(const std::shared_ptr<evpp::TCPClient>& pClient)
	: MPNetObject(pClient->conn()),m_pClient(pClient)
{
	m_nFD = -1;
	auto& sInfo = pClient->local_addr();
	auto nPos = sInfo.find_first_of(':');
	m_sIP = sInfo.substr(0, nPos);
	m_nPort = std::atoi(sInfo.substr(nPos + 1, sInfo.size()).c_str());
}*/

MPTCPClientObj::MPTCPClientObj(evpp::TCPClient* pClient)
: MPNetObject(pClient->conn()),m_pClient(pClient)
{
m_nFD = -1;
auto& sInfo = pClient->local_addr();
auto nPos = sInfo.find_first_of(':');
m_sIP = sInfo.substr(0, nPos);
m_nPort = std::atoi(sInfo.substr(nPos + 1, sInfo.size()).c_str());
}


MPTCPClientObj::~MPTCPClientObj()
{
	if(m_pClient != nullptr)
	{
		delete m_pClient;
		m_pClient = nullptr;
	}
}

/*std::shared_ptr<evpp::TCPClient>& MPTCPClientObj::GetClient()
{
	return m_pClient;
}*/
evpp::TCPClient* MPTCPClientObj::GetClient()
{
	return m_pClient;
}

void MPTCPClientObj::OnConnectCB()
{
	m_pConn = m_pClient->conn();
	m_nFD = m_pConn->fd();
}
