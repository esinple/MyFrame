#pragma once
#include "ManagerModule.h"
#include "StatsCommDef.h"
#include "MPTime.h"
#ifdef WIN_SYSTEM
#else
#include "DefaultMQProducer.h"

using namespace rocketmq;
#endif


class RocketMQManager final : public ManagerModule
{
public:
	RocketMQManager();
	~RocketMQManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
private:
	H_STATS_MANAGER_REG(RocketMQManager);
#ifdef WIN_SYSTEM
#else
    DefaultMQProducer m_producer;
#endif
};
