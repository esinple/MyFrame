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
	H_GAME_MANAGER_REG(eGameMgr_Time, GameTimeManager);

	meplay::MPTime m_CurTime;

	TimeManager m_TimeMgr;
};

#define GAME_CUR_TIME g_pGameNetProxy->GetModule<GameTimeManager>(eGameMgr_Time)->CurrentTime()

#define _MSEC_TICK_(x) this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::milliseconds(x))

#define _1_SEC_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::seconds(1))
#define _5_SEC_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::seconds(5))
#define _SEC_TICK_(x) this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::seconds(x))

#define _1_MIN_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::minutes(1))
#define _5_MIN_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::minutes(5))
#define _MIN_TICK_(x) this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::minutes(x))

#define _1_HOUR_TICK_ this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::hours(1))
#define _HOUR_TICK_(x) this->ModuleTimeTick::Tick(GAME_CUR_TIME,std::chrono::hours(x))