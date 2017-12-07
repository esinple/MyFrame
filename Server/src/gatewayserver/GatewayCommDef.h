#pragma once
#include "MPAutoRegister.h"
#include "GatewayManagerModuleDefine.h"
#include "GatewayNetProxy.h"
#include "MPDefine.pb.h"
#include "MPModuleFactory.h"


enum eGateAutoRegisterType
{
	eGateManagerModule = 1,
	eGateUserModule = 2,

};

#define GATE_MANAGER_MODULE_REG(INDEX,SUB) AUTO_REGISTER(eGateManagerModule,INDEX,SUB)
#define GATE_USER_MODULE_REG(INDEX,SUB) AUTO_REGISTER(eGateUserModule,INDEX,SUB)

#define GATE_CUR_TIME g_pGatewayNetProxy->GetModule<GateTimeManager>(eGateMgr_Time)->CurrentTime()

#define REGISTER_SUPER(CLASS,MSG,CALLBACK) g_pGatewayNetProxy->AddReceiveCallBack(MP_ST_SUPER,CLASS,MSG,&CALLBACK);
#define REGISTER_GAME(CLASS,MSG,CALLBACK) g_pGatewayNetProxy->AddReceiveCallBack(MP_ST_GAME,CLASS,MSG,&CALLBACK);
#define REGISTER_CLIENT(CLASS,CALLBACK) g_pGatewayNetProxy->AddReceiveCallBackEx(MP_CLIENT,CLASS,&CALLBACK);

#define SEND2CLIENT(FD,MSGID,MSG) g_pGatewayNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Client,MP_ST_GATE,MP_CLIENT,FD,MSGID,MSG);
#define SEND2CLIENT_ALL(MSGID,MSG) g_pGatewayNetProxy->SendMsgAll(MPMsg::MsgType::eMsgType_Client,MP_ST_GATE,MP_CLIENT,MSGID,MSG);
#define SEND2CLIENT_NOMSGID(FD,MSG) g_pGatewayNetProxy->SendMsg(MP_CLIENT,FD,MSG);
#define SEND2CLIENT_NOMSGID_ALL(MSG) g_pGatewayNetProxy->SendMsgAll(MP_CLIENT,MSG);
#define SEND2SUPER(FD,MSGID,MSG) g_pGatewayNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Gate2Super,MP_ST_GATE,MP_ST_SUPER,FD,MSGID,MSG);
#define SEND2GAME(FD,MSGID,MSG) g_pGatewayNetProxy->SendMsg(MPMsg::MsgType::eMsgType_Gate2Game,MP_ST_GATE,MP_ST_GAME,FD,MSGID,MSG);
#define SEND2GAME_ALL(MSGID,MSG) g_pGatewayNetProxy->SendMsgAll(MPMsg::MsgType::eMsgType_Gate2Game,MP_ST_GATE,MP_ST_GAME,MSGID,MSG);
