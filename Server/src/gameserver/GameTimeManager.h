#pragma once
#include "ManagerModule.h"
#include "GameCommDef.h"
#include "MPTime.h"
#include "TimeManager.h"

class GameTimeManager final : public ManagerModule
{
public:
	GameTimeManager();
	~GameTimeManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	inline const meplay::MPTime& CurrentTime()const { return m_CurTime; };
	inline TimeManager& GetTimeRegister() { return m_TimeMgr; };
private:

	meplay::MPTime m_CurTime;

	TimeManager m_TimeMgr;
};

GAME_MANAGER_MODULE_REG(eGameMgr_Time, GameTimeManager);
//DECLEAR_AUTO_REGISTER_SUB1();

//Time
#define GAME_CUR_TIME g_pGameNetProxy->GetModule<GameTimeManager>(eGameMgr_Time)->CurrentTime()
#define REGISTER_TIME_EVENT g_pGameNetProxy->GetModule<GameTimeManager>(eGameMgr_Time)->GetTimeRegister().AddTimeEvent

#define _MSEC_TICK_(x) this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::milliseconds(x))

#define _1_SEC_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::seconds(1))
#define _5_SEC_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::seconds(5))
#define _SEC_TICK_(x) this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::seconds(x))

#define _1_MIN_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::minutes(1))
#define _5_MIN_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::minutes(5))
#define _MIN_TICK_(x) this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::minutes(x))

#define _1_HOUR_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::hours(1))
#define _HOUR_TICK_(x) this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::hours(x))