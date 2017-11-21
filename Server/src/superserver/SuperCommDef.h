#pragma once
#include "MPAutoRegister.h"
#include "SuperManagerModuleDefine.h"
#include "SuperManagerModuleEnum.h"
#include "SuperNetProxy.h"
#include "MPDefine.pb.h"

#define H_SUPER_MANAGER_REG(SUB) H_AUTO_REGISTER_SUB(MPModule,SUB)
#define CPP_SUPER_MANAGER_REG(SUB) CPP_AUTO_REGISTER_SUB(SUB)

#define SUPER_CUR_TIME g_pSuperNetProxy->GetModule<SuperTimeManager>(eSuperMgr_Time)->CurrentTime()

#define REGISTER_GATE(CLASS,MSG,CALLBACK) g_pSuperNetProxy->AddReceiveCallBack(MP_ST_GATE,CLASS,MSG,&CALLBACK);
#define REGISTER_GAME(CLASS,MSG,CALLBACK) g_pSuperNetProxy->AddReceiveCallBack(MP_ST_GAME,CLASS,MSG,&CALLBACK);
#define REGISTER_CENTER(CLASS,MSG,CALLBACK) g_pSuperNetProxy->AddReceiveCallBack(MP_ST_CENTER,CLASS,MSG,&CALLBACK);

#define SEND2GATE(FD,MSGID,MSG) g_pSuperNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Super2Gate,MP_ST_SUPER,MP_ST_GATE,FD,MSGID,MSG);
#define SEND2GAME(FD,MSGID,MSG) g_pSuperNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Super2Game,MP_ST_SUPER,MP_ST_GAME,FD,MSGID,MSG);
#define SEND2CENTER(FD,MSGID,MSG) g_pSuperNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Super2Center,MP_ST_SUPER,MP_ST_CENTER,FD,MSGID,MSG);
