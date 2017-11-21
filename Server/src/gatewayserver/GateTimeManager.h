#pragma once
#include "ManagerModule.h"
#include "GatewayCommDef.h"
#include "MPTime.h"

class GateTimeManager final : public ManagerModule
{
public:
	GateTimeManager();
	~GateTimeManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	inline const meplay::MPTime& CurrentTime()const { return m_CurTime; };
private:
	H_GATE_MANAGER_REG(GateTimeManager);

	meplay::MPTime m_CurTime;
};