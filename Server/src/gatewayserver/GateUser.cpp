#include "GateUser.h"
#include "GatewayNetProxy.h"
#include "GateTimeManager.h"

GateUser::GateUser(uint64_t nSockIndex, const std::string& sIP)
{
	ReuseInit(nSockIndex,sIP);
}

GateUser::~GateUser()
{
}
void GateUser::ReuseInit(uint64_t nSockIndex, const std::string& sIP)
{
	m_nSockIndex = nSockIndex;
	m_sIP = sIP;
}

void GateUser::Clear()
{
}

uint64_t GateUser::GetFD()const
{
	return m_nSockIndex;
}