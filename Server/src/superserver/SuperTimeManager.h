#pragma once
#include "ManagerModule.h"
#include "SuperCommDef.h"
#include "MPTime.h"
#include "TimeManager.h"

class SuperTimeManager final : public ManagerModule
{
public:
	SuperTimeManager();
	~SuperTimeManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	inline const meplay::MPTime& CurrentTime()const { return m_CurTime; };
	inline TimeManager& GetTimeRegister() { return m_TimeMgr; };
public:
	
private:
	H_SUPER_MANAGER_REG(SuperTimeManager);

	meplay::MPTime m_CurTime;

	TimeManager m_TimeMgr;
};