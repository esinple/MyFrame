#include "SuperServer.h"
#include "GatewayNetProxy.h"
#include "GateTimeManager.h"
#include "MPMsg.h"
#include "Gate2Super.pb.h"
#include "MPDefine.pb.h"

SuperServer::SuperServer(uint64_t nSockIndex,const char* ip, int nPort)
{
	ReuseInit(nSockIndex, ip, nPort);
}

SuperServer::~SuperServer()
{
}

void SuperServer::ReuseInit(uint64_t nSockIndex, const char* ip, int nPort)
{
	m_nSockIndex = nSockIndex;
	m_sIP = ip;
	m_nPort = nPort;
	m_LogonTime = GATE_CUR_TIME;
	m_VerifyTime = m_LogonTime;
}

void SuperServer::Clear()
{
}

void SuperServer::VerifySuperServer()
{
	//Message msg;
	if (GATE_CUR_TIME < m_VerifyTime)
	{
		return;
	}
	m_VerifyTime += 10;
	MPMsg::GateVerify_Gate2Super msg;
	SEND2SUPER(m_nSockIndex,MPMsg::eGate2Super_VerifyMe, msg);
}