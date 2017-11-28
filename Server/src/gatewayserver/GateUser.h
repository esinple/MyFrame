#pragma once
#include <memory>
#include "MPGUID.h"
#include "MPTime.h"

class GateUser final
{
public:
	GateUser(uint64_t nSockIndex,const std::string& sIP);
	~GateUser();
public:
	void ReuseInit(uint64_t nSockIndex, const std::string& sIP);
	void Clear();

	uint64_t GetFD()const;

	inline const meplay::MPTime& GetLoginTime()const { return m_LoginTime; };
	inline void SetLoginTime(const meplay::MPTime& time){ m_LoginTime = time; };
private:
	//ÓÃ»§Î¨Ò»id
	MPGUID m_uid;
	uint64_t m_nSockIndex;
	std::string m_sIP;

	meplay::MPTime m_LoginTime;
};

typedef std::shared_ptr<GateUser> GateUserPtr;