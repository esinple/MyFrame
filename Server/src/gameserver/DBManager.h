#pragma once
#include "GameCommDef.h"
#include <google/protobuf/message.h>
#include "DBClientMgr.h"

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
	void SaveToDB(google::protobuf::Message& msg);
	void SaveToDB(uint32_t nType,google::protobuf::Message& msg);
private:
	H_GAME_MANAGER_REG(DBManager)
};

#define g_pDBMgr DBClientMgr::GetInstance()->GetConn(eGameDB_Normal)