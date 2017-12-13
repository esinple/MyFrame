#include "TestManager.h"
#include "MPDefine.pb.h"
#include "FCManagerModuleDefine.h"
#include "FCNetProxy.h"
#include "UserBase.pb.h"
#include "UserMsgDefine.pb.h"
#include "UserErrCode.pb.h"

#define SEND2GATE(FD,MSGID,MSG) g_pFCNetProxy->SendMsg(MPMsg::eMsgType_Client, MP_CLIENT, MP_ST_GATE, FD, MSGID, MSG);
#define REGISTER_GATE(CLASS,MSG,CALLBACK) g_pFCNetProxy->AddReceiveCallBack(MP_ST_GATE,CLASS,MSG,&CALLBACK);


TestManager::TestManager()
	: ManagerModule(eFCMgr_Test)
{
}

TestManager::~TestManager()
{
}
bool TestManager::Awake()
{
	REGISTER_GATE(this, UserMsg_Logon, TestManager::UserLogonCB);
	return true;
}
bool TestManager::AfterAwake()
{
	return true;
}
bool TestManager::Execute()
{
	switch (m_nStatus)
	{
	case eCS_Connected:
	{
		TestConnect();
	}
	break;
	case eCS_Unknown:
	{
	}
	break;
	}
	return true;
}
bool TestManager::BeforeShutDown()
{
	return true;
}
bool TestManager::ShutDown()
{
	return true;
}

void TestManager::TestConnect()
{
	std::string sAccount, sPassword;
	std::cin >> sAccount;
	std::cin >> sPassword;
	UserLogon_C2S msg;
	msg.set_account(sAccount);
	msg.set_password(sPassword);

	SEND2GATE(m_fd, UserMsg_Logon, msg);

	m_nStatus = eCS_Unknown;
}

void TestManager::UserLogonCB(const std::string& data, const uint64_t nSockIndex)
{
	UserLogon_S2C inMsg;
	bool b = inMsg.ParseFromString(data);

	m_nStatus = eCS_Unknown;
}