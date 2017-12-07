#pragma once
#include "MPAutoRegister.h"
#include "StatsManagerModuleDefine.h"
#include "StatsNetProxy.h"
#include "MPModuleFactory.h"

enum eStatsAutoRegisterType
{
	eStatsManagerModule = 1,
	eStatsUserModule = 2,

};

#define STATS_MANAGER_MODULE_REG(INDEX,SUB) AUTO_REGISTER(eStatsManagerModule,INDEX,SUB)
#define STATS_USER_MODULE_REG(INDEX,SUB) AUTO_REGISTER(eStatsUserModule,INDEX,SUB)


#define H_STATS_MANAGER_REG(INDEX,SUB) H_AUTO_REGISTER_SUB(INDEX,MPModule,SUB)
#define CPP_STATS_MANAGER_REG(SUB) CPP_AUTO_REGISTER_SUB(SUB)

#define SUPER_CUR_TIME g_pStatsNetProxy->GetModule<StatsTimeManager>(eStatsMgr_Time)->CurrentTime()

//#define RegisterGate2Stats(CLASS,MSG,CALLBACK) g_pStatsNetProxy->AddReceiveCallBack(MP_ST_GATE,CLASS,MSG,&CALLBACK);
//#define RegisterGame2Stats(CLASS,MSG,CALLBACK) g_pStatsNetProxy->AddReceiveCallBack(MP_ST_GAME,CLASS,MSG,&CALLBACK);
//#define RegisterCenter2Stats(CLASS,MSG,CALLBACK) g_pStatsNetProxy->AddReceiveCallBack(MP_ST_CENTER,CLASS,MSG,&CALLBACK);

//#define SEND2GATE(FD,MSGID,MSG) g_pStatsNetProxy->SendMsg(MP_ST_SUPER,MP_ST_GATE,FD,MSGID,MSG);
