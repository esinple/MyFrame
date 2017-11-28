#include "StatsServer.h"
#include "GameNetProxy.h"
#include "GameTimeManager.h"
#include "MPMsg.h"
#include "MPDefine.pb.h"

StatsServer::StatsServer(uint64_t nSockIndex,const char* ip, int nPort)
	//: m_nSockIndex(nSockIndex),m_sIP(ip), m_nPort(nPort)
{
	//auto pGateTimeMgr = g_pGatewayNetProxy->GetModule<GateTimeManager>(eGatewayMgr_Time);
	//m_LoginTime = pGateTimeMgr->CurrentTime();
	ReuseInit(nSockIndex, ip, nPort);
}

StatsServer::~StatsServer()
{
}

void StatsServer::ReuseInit(uint64_t nSockIndex, const char* ip, int nPort)
{
	m_nSockIndex = nSockIndex;
	m_sIP = ip;
	m_nPort = nPort;

	m_LoginTime = GAME_CUR_TIME;
	m_VerifyTime = m_LoginTime;
}

void StatsServer::Clear()
{
}