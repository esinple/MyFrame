#pragma once
#include <memory>
#include "MPTime.h"

class GameServer final
{
public:
	GameServer(uint64_t nSockIndex, const char* ip, int nPort);
	~GameServer();
public:
	inline const std::string& GetIP()const { return m_sIP; }
	inline const int GetPort()const { return m_nPort; }
public:
private:
	const uint64_t m_nSockIndex;
	std::string m_sIP;
	const int	m_nPort;
};

typedef std::shared_ptr<GameServer> GameServerPtr;