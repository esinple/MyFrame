#pragma once
#include <memory>
#include "MPTime.h"

class GateServer final
{
public:
	GateServer(uint64_t nSockIndex, const char* ip, int nPort);
	~GateServer();
public:
	inline const std::string& GetIP()const { return m_sIP; }
	inline const int GetPort()const { return m_nPort; }
public:
private:
	const uint64_t m_nSockIndex;
	std::string m_sIP;
	const int	m_nPort;
	meplay::MPTime m_LoginTime;
	meplay::MPTime m_VerifyTime;
};

typedef std::shared_ptr<GateServer> GateServerPtr;