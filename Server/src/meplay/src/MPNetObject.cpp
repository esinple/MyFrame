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

MPNetObject::MPNetObject()
{
	
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

MPTCPObject::MPTCPObject(const std::shared_ptr<evpp::TCPConn>& pConn)
	: MPNetObject()
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

MPTCPObject::~MPTCPObject()
{
}

void MPTCPObject::Send(const char * data, uint32_t len)
{
	m_pConn->Send(data, len);
}

void MPTCPObject::Close()
{
	m_pConn->Close();
}

std::shared_ptr<evpp::TCPConn>& MPTCPObject::GetConn()
{
	return m_pConn;
}

MPRUDPObject::MPRUDPObject(ENetPeer* pPeer)
{
	m_nFD = createRUDPFD();
	//in_addr addr;
	//addr.S_un.S_addr = pPeer->address.host;
	m_sIP = inet_ntoa(*(struct in_addr *)&pPeer->address.host);
	m_nPort = pPeer->address.port;
	m_pPeer = pPeer;
}

MPRUDPObject::~MPRUDPObject()
{
}

void MPRUDPObject::Send(const char * data, uint32_t len)
{
	auto pPacket = enet_packet_create(data, len, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(m_pPeer, 0, pPacket);
	enet_host_flush(m_pPeer->host);
}

void MPRUDPObject::Close()
{
	enet_peer_disconnect_now(m_pPeer,0);
}

ENetPeer* MPRUDPObject::GetENetPeer()
{
	return m_pPeer;
}

uint32_t MPRUDPObject::m_nSeed = 0;
uint32_t MPRUDPObject::createRUDPFD()
{
	return ++m_nSeed;
}