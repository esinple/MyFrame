#include "MPRUDPServer.h"
#include "MPLogger.h"

using namespace meplay;
#define DEFAULT_TIME_OUT 3000

MPRUDPServer::~MPRUDPServer()
{
}
bool MPRUDPServer::Execute()
{
	MPThread::ThreadStart();
	if (0)
	{
		auto mNetObj = GetAllNetObject();
		for (auto&&[nSockIndex, pNetObject] : mNetObj)
		{
			(pNetObject.get())->Close();
		}
	}
	static bool flag = true;
	if (flag)
	{
		std::string s("abdsfafadsfa");
		SendMsgToAllWithOutHead(123, s.c_str(), s.size());
	}
	if(0)
	{
		Final();
	}
	return true;
}

int MPRUDPServer::InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)
{
	return -1;
}

int MPRUDPServer::InitializationAsServer(const char * strIP, const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount)
{
	if (enet_initialize() != 0)
	{
		MP_ERROR("An error occurred while initializing ENet.");
		return -1;
	}

	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	m_ENetAddress.host = ENET_HOST_ANY;
	m_ENetAddress.port = nPort;
	m_nPort = nPort;
	m_pENetServer = enet_host_create(
		&m_ENetAddress,			/* the address to bind the server host to */
		nMaxClient,				/* allow up to 32 clients and/or outgoing connections */
		1,						/* allow up to 2 channels to be used, 0 and 1 */
		0,						/* assume any amount of incoming bandwidth */
		0						/* assume any amount of outgoing bandwidth */
		);
	if (m_pENetServer == nullptr)
	{
		MP_ERROR("An error occurred while trying to create an ENet server host.");
		return -1;
	}
	return 0;
}

bool MPRUDPServer::Final()
{
	auto mNetObj = GetAllNetObject();
	for (auto&&[nSockIndex, pNetObject] : mNetObj)
	{
		(pNetObject.get())->Close();
	}

	enet_host_destroy(m_pENetServer);

	MPThread::ThreadFinal();
	return true;
}

bool MPRUDPServer::SendMsgWithOutHead(
	const int16_t nMsgID, 
	const char* msg, 
	const uint32_t nLen, 
	const MPSOCK nSockIndex
)
{

	auto pPacket = createPacket(nMsgID, msg, nLen);

	auto pNetObj = (MPRUDPObject*)GetNetObject(nSockIndex).get();
	
	enet_peer_send(pNetObj->GetENetPeer(), 0, pPacket);

	return true;
}

bool MPRUDPServer::SendMsgToAllWithOutHead(
	const int16_t nMsgID,
	const char* msg,
	const uint32_t nLen
)
{
	auto pPacket = createPacket(nMsgID, msg, nLen);

	enet_host_broadcast(m_pENetServer, 0, pPacket);

	return true;
}

bool MPRUDPServer::SendMsgWithOutHead(
	const int16_t nMsgID,
	const char* msg,
	const uint32_t nLen,
	const std::list<MPSOCK>& fdList
)
{
	auto pPacket = createPacket(nMsgID, msg, nLen);

	for (auto nSockIndex : fdList)
	{
		auto pNetObj = (MPRUDPObject*)GetNetObject(nSockIndex).get();

		enet_peer_send(pNetObj->GetENetPeer(), 0, pPacket);
	}
	//enet_host_flush(m_pENetServer);

	return true;
}

void MPRUDPServer::connectCB(ENetEvent* pEvent)
{
	MPNetObjectPtr pNetObject = std::make_shared<MPRUDPObject>(pEvent->peer);
	AddNetObject(pNetObject->GetRealFD(), pNetObject);
	pEvent->peer->data = pNetObject.get();
	enet_peer_timeout(pEvent->peer, 10, DEFAULT_TIME_OUT, DEFAULT_TIME_OUT);

	m_ConnectCB(GetServerType(), pNetObject->GetRealFD());
}

void MPRUDPServer::disConnectCB(ENetEvent* pEvent)
{
	auto pNetObj = (MPRUDPObject*)pEvent->peer->data;
	if (pNetObj == nullptr)
	{
		return;
	}

	m_DisConnectCB(GetServerType(), pNetObj->GetRealFD());
	DelNetObject(pNetObj->GetRealFD());
	pEvent->peer->data = nullptr;
}

void MPRUDPServer::messageCB(ENetEvent* pEvent)
{
	auto pNetObj = (MPRUDPObject*)pEvent->peer->data;
	m_MsgCB(GetServerType(), pNetObj->GetRealFD(), (char*)pEvent->packet->data, (uint32_t)pEvent->packet->dataLength);
	/* Clean up the packet now that we're done using it. */
	enet_packet_destroy(pEvent->packet);
}

ENetPacket* MPRUDPServer::createPacket(const int16_t nMsgID, const char* msg, const uint32_t nLen)
{
	std::string strOutData;
	uint32_t nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
	auto pPacket = enet_packet_create(strOutData.c_str(), nAllLen, ENET_PACKET_FLAG_RELIABLE);
	return pPacket;
}

void MPRUDPServer::Run()
{
	while (!IsThreadFinal())
	{
		ENetEvent e;
		while (enet_host_service(m_pENetServer, &e, 0) > 0)
		{
			switch (e.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				connectCB(&e);
			}
			break;
			case ENET_EVENT_TYPE_RECEIVE:
			{
				messageCB(&e);
			}
			break;
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				disConnectCB(&e);
			}
			break;
			default:
				break;
			}
		}
	}
}