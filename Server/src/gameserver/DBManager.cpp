#include "DBManager.h"
#include "DBClientMgr.h"

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
	//g_pDBMgr->RegisterIndex()
	//config maybe?
	if (!g_pDBMgr->Connect(eGameDB_Normal, "172.24.3.89", 27017, "ajiang_test"))
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
	if (g_pDBMgr->ExecInsert(eGameDB_Normal, msg) <= 0)
	{
		g_pDBMgr->ExecUpdate(eGameDB_Normal, msg, msg);
	}
}

void DBManager::SaveToDB(uint32_t nType,google::protobuf::Message& msg)
{
	if (g_pDBMgr->ExecInsert(nType, msg) <= 0)
	{
		g_pDBMgr->ExecUpdate(nType, msg, msg);
	}
}