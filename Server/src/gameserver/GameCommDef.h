#pragma once
#include "MPAutoRegister.h"
#include "GameManagerModuleDefine.h"
#include "GameNetProxy.h"
#include "UserErrCode.pb.h"

#define H_GAME_MANAGER_REG(INDEX,SUB) H_AUTO_REGISTER_SUB(INDEX,MPModule,SUB)
#define CPP_GAME_MANAGER_REG(SUB) CPP_AUTO_REGISTER_SUB(SUB)

#define REGISTER_GATE(CLASS,MSG,CALLBACK) g_pGameNetProxy->AddReceiveCallBack(MP_ST_GATE,CLASS,MSG,&CALLBACK);
#define REGISTER_SUPER(CLASS,MSG,CALLBACK) g_pGameNetProxy->AddReceiveCallBack(MP_ST_SUPER,CLASS,MSG,&CALLBACK);

#define SEND_USER_MSG(FD,MSGID,MSG) g_pGameNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Game2Gate,MP_ST_GAME,MP_ST_GATE,FD,MSGID,MSG);
#define SEND2SUPER(FD,MSGID,MSG) g_pGameNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Game2Super,MP_ST_GAME,MP_ST_SUPER,FD,MSGID,MSG);
#define SEND2GATE(FD,MSGID,MSG) g_pGameNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Game2Gate,MP_ST_GAME,MP_ST_GATE,FD,MSGID,MSG);
