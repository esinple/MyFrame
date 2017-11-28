#include "DBManager.h"

CPP_GAME_MANAGER_REG(DBManager)

DBManager::DBManager()
	: ManagerModule(eGameMgr_DBServer)
{
}

DBManager::~DBManager()
{
}

bool DBManager::Awake()
{
	DBClientMgr::GetInstance()->RegisterIndex(eGameDB_Normal, CREATE_INDEX("UserDB.GameUserLogon", KEYS("account"), true));
	//config maybe?
	if (!DBClientMgr::GetInstance()->Connect(eGameDB_Normal, "172.24.3.89", 27017, "ajiang_test"))
	{
		return false;
	}

	return true;
}
bool DBManager::AfterAwake()
{

	return true;
}
bool DBManager::Execute()
{
	return true;
}
bool DBManager::BeforeShutDown()
{
	return true;
}
bool DBManager::ShutDown()
{
	return true;
}

void DBManager::SaveToDB(google::protobuf::Message& msg)
{
	if (g_pDBMgr->ExecInsert(msg) <= 0)
	{
		g_pDBMgr->ExecUpdate( msg, msg);
	}
}

void DBManager::SaveToDB(uint32_t nType,google::protobuf::Message& msg)
{
	if (DBClientMgr::GetInstance()->ExecInsert(nType, msg) <= 0)
	{
		DBClientMgr::GetInstance()->ExecUpdate(nType, msg, msg);
	}
}