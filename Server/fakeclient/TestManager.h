#pragma once
#include "ManagerModule.h"
#include "MPModuleFactory.h"
#include "FCManagerModuleEnum.h"

enum ClientStatus
{
	eCS_Unknown			= 0,
	eCS_Connected		= 1,
};

class TestManager : public ManagerModule
{
public:
	TestManager();
	~TestManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	void SetStatus(int nStatus) { m_nStatus = nStatus; };
	void SetSockIndex(const uint64_t nSockIndex) { m_fd = nSockIndex; }

	void TestConnect();

	void UserLogonCB(const std::string&, const uint64_t nSockIndex);
private:

	int m_nStatus = eCS_Unknown;
	uint64_t m_fd;
};

AUTO_REGISTER(0,eFCMgr_Test, TestManager);