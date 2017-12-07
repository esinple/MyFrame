#pragma once
#include "ManagerModule.h"
#include "GatewayCommDef.h"
#include "GateUser.h"
#include "MPNetDefine.h"

class GateUserManager final: public ManagerModule
{
public:
	GateUserManager();
	~GateUserManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	void GateUserConnect(uint64_t nSockIndex,const std::string& sIP);
	void GateUserDisConnect(uint64_t nSockIndex);

	GateUserPtr GetTempGateUser(uint64_t nSockIndex);

	const MPGUID GetGUID(uint64_t nSockIndex)const;

	bool OnUser2GameMsgWrapperSock(MPMsg::MsgBase& msg_base, const uint64_t nSockIndex);
	bool OnUser2GameMsgWrapperUID(MPGUID uid, MPMsg::MsgBase& msg_base);
	void OnGame2UserMsgWrapperSock(const std::string& data, const uint64_t nSockIndex);
	void OnGame2UserMsgWrapperUID(MPGUID uid, const std::string& data);
	void OnGameUserLogonSuccess(const std::string& data,const uint64_t nSockIndex);

	GateUserPtr GetGateUser(MPGUID uid);

	void KickAll();
private:
	void checkTempGateUsers();

	void addTempGateUser(uint64_t nSockIndex,const std::string& sIP);
	void delTempGateUser(uint64_t nSockIndex);

	void addGateUser(MPGUID uid, uint64_t nSockIndex,GateUserPtr pGateUser);
	void delGateUser(MPGUID uid);

	void onGateUserDisConnect(MPGUID uid);
private:
	MPMemoryPool<GateUser> m_objGateUserPool;
	std::unordered_map<uint64_t, GateUserPtr> m_mTempGateUsers;
	std::unordered_map<MPGUID, GateUserPtr> m_mGateUsers;
	std::unordered_map<uint64_t, MPGUID> m_mSock2GUID;

	std::mutex mtx;
};

GATE_MANAGER_MODULE_REG(eGateMgr_GateUser, GateUserManager);
