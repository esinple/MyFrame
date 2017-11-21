#include "RocketMQManager.h"
#include "MPLogger.h"

CPP_STATS_MANAGER_REG(RocketMQManager);

RocketMQManager::RocketMQManager()
	: ManagerModule(eStatsMgr_Time)
#ifdef WIN_SYSTEM
#else
    ,m_producer("test_group_name")
#endif
{
}

RocketMQManager::~RocketMQManager()
{
}
bool RocketMQManager::Awake()
{
#ifdef WIN_SYSTEM
#else
    m_producer.start();
#endif
	return true;
}

bool RocketMQManager::AfterAwake()
{
	return true;
}

bool RocketMQManager::Execute()
{
#ifdef WIN_SYSTEM
#else
    if(1)
    {
        MQMessage msg("test_topic",  // topic
                "*",          // tag
                "this is body");  // body
        SendResult sendResult = m_producer.send(msg,false);
        if(sendResult.getSendStatus() != SEND_OK)
        {
            sendResult.getMsgId().c_str();
        }
    }
#endif
    return true;
}

bool RocketMQManager::BeforeShutDown()
{
    return true;
}

bool RocketMQManager::ShutDown()
{
#ifdef WIN_SYSTEM
#else
    m_producer.shutdown();
#endif
    return true;
}
