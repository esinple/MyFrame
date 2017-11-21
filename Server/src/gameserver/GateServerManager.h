#pragma once
#include "GameCommDef.h"
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
	void AddGateServer(uint64_t nSockIndex, const char* ip, int nPort);
	void DelGateServer(uint64_t nSockIndex);
	GateServerPtr GetGateServer(uint64_t nSockIndex);

	void OnGateUserMsg(const std::string& sData,const uint64_t nSockIndex);
	void OnGateUserLogout(const std::string& sData,const uint64_t nSockIndex);

	void SendToGate(uint64_t nSockIndex,uint32_t nMsgId,google::protobuf::Message& pb);
	void SendToGateAll(uint32_t nMsgId,google::protobuf::Message& pb);
private:
private:
	H_GAME_MANAGER_REG(GateServerManager);

	std::map<const uint64_t, GateServerPtr> m_mGateServers;

};
