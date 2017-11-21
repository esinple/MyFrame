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
	H_GAME_MANAGER_REG(GameTimeManager);

	meplay::MPTime m_CurTime;

	TimeManager m_TimeMgr;
};