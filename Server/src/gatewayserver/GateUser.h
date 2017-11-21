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

	inline const meplay::MPTime& GetLogonTime()const { return m_LogonTime; };
	inline void SetLogonTime(const meplay::MPTime& time){ m_LogonTime = time; };
private:
	//ÓÃ»§Î¨Ò»id
	MPGUID m_uid;
	uint64_t m_nSockIndex;
	std::string m_sIP;

	meplay::MPTime m_LogonTime;
};

typedef std::shared_ptr<GateUser> GateUserPtr;