#pragma once
#include "SuperCommDef.h"
#include "GateServer.h"

class GateServerManager final : public ManagerModule
{
public:
	GateServerManager();
	~GateServerManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	void OnGateVerify(const std::string& sData,const uint64_t nSockIndex);
	void AddGateServer(uint64_t nSockIndex, const char* ip, int nPort);
	void DelGateServer(uint64_t nSockIndex);
private:
	H_SUPER_MANAGER_REG(eSuperMgr_GateServer,GateServerManager);

	std::map<const uint64_t, GateServerPtr> m_mGateServers;

};