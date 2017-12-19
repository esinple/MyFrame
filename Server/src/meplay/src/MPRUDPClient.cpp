#include "MPRUDPClient.h"
#include "MPLogger.h"

using namespace meplay;

MPRUDPClient::~MPRUDPClient()
{
}
bool MPRUDPClient::Execute()
{
	MPThread::ThreadStart();
	if (0)
	{
		Final();
	}
	if (0)
	{
		std::string s("abdsfafadsfa");
		SendMsgToAllWithOutHead(123, s.c_str(), (uint32_t)s.size());
	}
	return true;
}

int MPRUDPClient::InitializationAsClient(const char* strIP, const unsigned short nPort, bool bAutoReconnect, const uint32_t milliseconds)
{
	RUDPConnectData cd;
	if (enet_initialize() != 0)
	{
		MP_ERROR("An error occurred while initializing ENet.");
		return -1;
	}

	cd.m_pENetClient = enet_host_create(nullptr,	/* create a client host */
		2048,									/* only allow 1 outgoing connection */
		1,										/* allow up 2 channels to be used, 0 and 1 */
		0,										/* 56K modem with 56 Kbps downstream bandwidth */
		0										/* 56K modem with 14 Kbps upstream bandwidth */
	);
	if (cd.m_pENetClient == nullptr)
	{
		MP_ERROR("An error occurred while trying to create an ENet client host.");
		return -1;
	}

	enet_address_set_host(&cd.m_ENetAddress, strIP);
	cd.m_ENetAddress.port = nPort;
	cd.m_nPort = nPort;
	cd.m_bAutoConnect = bAutoReconnect;
	cd.m_nTimeOut = milliseconds;

	tryConnect(cd);
	
	m_vConnects.emplace_back(cd);

	return 0;
}

int MPRUDPClient::InitializationAsServer(const char * strIP, const unsigned int nMaxClient, const unsigned short nPort, const int nThreadCount)
{
	return -1;
}

bool MPRUDPClient::Final()
{
	auto mNetObj = GetAllNetObject();
	for (auto&&[nSockIndex, pNetObject] : mNetObj)
	{
		(pNetObject.get())->Close();
	}

	for (auto& cd : m_vConnects)
	{
		enet_host_destroy(cd.m_pENetClient);
	}

	MPThread::ThreadFinal();
	return true;
}

bool MPRUDPClient::SendMsgWithOutHead(
	const int16_t nMsgID,
	const char* msg,
	const uint32_t nLen,
	const MPSOCK nSockIndex
)
{

	auto pPacket = createPacket(nMsgID, msg, nLen);

	auto pNetObj = (MPRUDPObject*)GetNetObject(nSockIndex).get();

	enet_peer_send(pNetObj->GetENetPeer(), 0, pPacket);

	/* One could just use enet_host_service() instead. */
	//enet_host_flush(pNetObj->GetENetPeer()->host);
	return true;
}

bool MPRUDPClient::SendMsgToAllWithOutHead(
	const int16_t nMsgID,
	const char* msg,
	const uint32_t nLen
)
{
	auto pPacket = createPacket(nMsgID, msg, nLen);

	for (auto& cd : m_vConnects)
	{
		enet_peer_send(cd.m_pENetClient->peers, 0, pPacket);

		//enet_host_flush(cd.m_pENetClient);
	}
	return true;
}

bool MPRUDPClient::SendMsgWithOutHead(
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

		//enet_host_flush(pNetObj->GetENetPeer()->host);
	}

	return true;
}

void MPRUDPClient::tryConnect(RUDPConnectData& data)
{
	/* Initiate the connection, allocating the two channels 0 and 1. */
	auto peer = enet_host_connect(data.m_pENetClient, &data.m_ENetAddress, 1, 0);
	if (peer == NULL)
	{
		MP_ERROR("No available peers for initiating an ENet connection.");
	}
	enet_peer_timeout(peer, 10, data.m_nTimeOut, data.m_nTimeOut);

}

void MPRUDPClient::connectCB(ENetEvent* pEvent)
{
	MPNetObjectPtr pNetObject = std::make_shared<MPRUDPObject>(pEvent->peer);
	AddNetObject(pNetObject->GetRealFD(), pNetObject);
	pEvent->peer->data = pNetObject.get();

	m_ConnectCB(GetServerType(), pNetObject->GetRealFD());
}

void MPRUDPClient::disConnectCB(ENetEvent* pEvent)
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

void MPRUDPClient::messageCB(ENetEvent* pEvent)
{
	auto pNetObj = (MPRUDPObject*)pEvent->peer->data;
	m_MsgCB(GetServerType(), pNetObj->GetRealFD(), (char*)pEvent->packet->data, (uint32_t)pEvent->packet->dataLength);
	/* Clean up the packet now that we're done using it. */
	enet_packet_destroy(pEvent->packet);
}

ENetPacket* MPRUDPClient::createPacket(const int16_t nMsgID, const char* msg, const uint32_t nLen)
{
	std::string strOutData;
	uint32_t nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
	auto pPacket = enet_packet_create(strOutData.c_str(), nAllLen, ENET_PACKET_FLAG_RELIABLE);
	return pPacket;
}

void MPRUDPClient::Run()
{
	while (!IsThreadFinal())
	{
		for (auto& cd : m_vConnects)
		{
			if (cd.m_pENetClient != nullptr && cd.m_bAutoConnect 
				&& cd.m_pENetClient->peers->state == ENET_PEER_STATE_DISCONNECTED)
			{
				tryConnect(cd);
			}

			ENetEvent e;
			while (enet_host_service(cd.m_pENetClient, &e, 0) > 0)
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
}