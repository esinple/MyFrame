#include "GameServer.h"
#include "GatewayNetProxy.h"
#include "GateTimeManager.h"

GameServer::GameServer(uint64_t nSockIndex,const char* ip, int nPort)
{
	ReuseInit(nSockIndex, ip, nPort);
}

GameServer::~GameServer()
{
}

void GameServer::ReuseInit(uint64_t nSockIndex, const char* ip, int nPort)
{
	m_nSockIndex = nSockIndex;
	m_sIP = ip;
	m_nPort = nPort;
	auto pGateTimeMgr = g_pGatewayNetProxy->GetModule<GateTimeManager>(eGatewayMgr_Time);
	m_LoginTime = pGateTimeMgr->CurrentTime();
}