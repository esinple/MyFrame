#pragma once
#include <string>
#include <stdint.h>
#include <memory>
#include "MPTime.h"

class GameServer final
{
public:
	GameServer(uint64_t nSockIndex,const char* ip,int nPort);
	~GameServer();
public:
	inline const std::string& GetIP()const { return m_sIP; }
	inline const int GetPort()const { return m_nPort; }
public:
	void ReuseInit(uint64_t nSockIndex,const char* ip,int nPort);
	void Clear() {};
private:
	uint64_t m_nSockIndex;
	std::string m_sIP;
	int	m_nPort;
	meplay::MPTime m_LogonTime;
};

typedef std::shared_ptr<GameServer> GameServerPtr;