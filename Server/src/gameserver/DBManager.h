#pragma once
#include "GameCommDef.h"
#include <google/protobuf/message.h>
#include "DBClientMgr.h"
#include "DBGameServer.pb.h"

enum GameDBType
{
	eGameDB_Normal		= 1,
};

class DBManager final : public ManagerModule
{
public:
	DBManager();
	~DBManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	void SaveToDB(google::protobuf::Message& filter, google::protobuf::Message& msg, bool all = false);
	void SaveToDB(uint32_t nType, google::protobuf::Message& filter, google::protobuf::Message& msg, bool all = false);
private:
	void registerDBKeys();
private:
};

GAME_MANAGER_MODULE_REG(eGameMgr_DBServer, DBManager);

#define g_pDBMgr DBClientMgr::GetInstance()->GetConn(eGameDB_Normal)
#define SAVE2DB(filter,msg) g_pGameNetProxy->GetModule<DBManager>(eGameMgr_DBServer)->SaveToDB(filter,msg);