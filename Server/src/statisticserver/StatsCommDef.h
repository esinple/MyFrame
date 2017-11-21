#pragma once
#include "MPAutoRegister.h"
#include "StatsManagerModuleDefine.h"
#include "StatsManagerModuleEnum.h"
#include "StatsNetProxy.h"

#define H_STATS_MANAGER_REG(SUB) H_AUTO_REGISTER_SUB(MPModule,SUB)
#define CPP_STATS_MANAGER_REG(SUB) CPP_AUTO_REGISTER_SUB(SUB)

#define SUPER_CUR_TIME g_pStatsNetProxy->GetModule<StatsTimeManager>(eStatsMgr_Time)->CurrentTime()

//#define RegisterGate2Stats(CLASS,MSG,CALLBACK) g_pStatsNetProxy->AddReceiveCallBack(MP_ST_GATE,CLASS,MSG,&CALLBACK);
//#define RegisterGame2Stats(CLASS,MSG,CALLBACK) g_pStatsNetProxy->AddReceiveCallBack(MP_ST_GAME,CLASS,MSG,&CALLBACK);
//#define RegisterCenter2Stats(CLASS,MSG,CALLBACK) g_pStatsNetProxy->AddReceiveCallBack(MP_ST_CENTER,CLASS,MSG,&CALLBACK);

//#define SEND2GATE(FD,MSGID,MSG) g_pStatsNetProxy->SendMsg(MP_ST_SUPER,MP_ST_GATE,FD,MSGID,MSG);
