#pragma once
#include "GatewayCommDef.h"
#include "ManagerModule.h"
#include "SuperServer.h"

class SuperServerManager : public ManagerModule
{
public:
	SuperServerManager();
	~SuperServerManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	void OnSuperVerify(const std::string& data,const uint64_t nSockIndex);

public:
	void AddSuperServer(uint64_t nSockIndex, const char* ip, int nPort);
	void DelSuperServer(uint64_t nSockIndex);
private:
	void checkVerifySuperServer();
private:
	H_GATE_MANAGER_REG(eGateMgr_SuperServer,SuperServerManager);

	MPMemoryPool<SuperServer> m_SuperServerPool;

	std::map<const uint64_t, SuperServerPtr> m_mSuperServers;
	std::map<const uint64_t, SuperServerPtr> m_mTempSuperServers;

	std::mutex mtx;
};