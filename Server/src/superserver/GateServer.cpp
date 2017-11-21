#include "GateServer.h"

GateServer::GateServer(uint64_t nSockIndex, const char* ip, int nPort)
	: m_nSockIndex(nSockIndex),m_sIP(ip),m_nPort(nPort)
{
}

GateServer::~GateServer()
{
}