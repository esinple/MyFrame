#include "DBManager.h"

DBManager::DBManager()
	: ManagerModule(eGameMgr_DBServer)
{
}

DBManager::~DBManager()
{
}

bool DBManager::Awake()
{
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

void DBManager::SaveToDB(google::protobuf::Message& filter, google::protobuf::Message& msg, bool all)
{
	if (g_pDBMgr->ExecInsert(msg) <= 0)
	{
		g_pDBMgr->ExecUpdate(filter, msg, all);
	}
}

void DBManager::SaveToDB(uint32_t nType, google::protobuf::Message& filter, google::protobuf::Message& msg, bool all)
{
	if (DBClientMgr::GetInstance()->ExecInsert(nType, msg) <= 0)
	{
		DBClientMgr::GetInstance()->ExecUpdate(nType, filter, msg, all);
	}
}

void DBManager::registerDBKeys()
{
	//keys should be lower case
	DBClientMgr::GetInstance()->RegisterIndex(eGameDB_Normal, CREATE_INDEX("GameUserLogon", KEYS("account"), true));
	DBClientMgr::GetInstance()->RegisterIndex(eGameDB_Normal, CREATE_INDEX("TimeStamp", KEYS("id","main_type","sub_type"), true));
}