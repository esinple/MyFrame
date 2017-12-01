#include "TimeStampManager.h"
#include "DBManager.h"

CPP_GAME_MANAGER_REG(TimeStampManager);

TimeStampManager::TimeStampManager() : ManagerModule(eGameMgr_TimeStamp)
{
	AddParent(eGameMgr_DBServer);
}

TimeStampManager::~TimeStampManager()
{
}

bool TimeStampManager::Awake()
{
	loadTimeStamp();
	return true;
}

bool TimeStampManager::AfterAwake()
{
	return true;
}

bool TimeStampManager::Execute()
{
	return true;
}

bool TimeStampManager::BeforeShutDown()
{
	return true;
}

bool TimeStampManager::ShutDown()
{
	return true;
}

void TimeStampManager::UpdateTimeStamp(MPGUID lUserId, uint32_t nMainType, uint32_t nSubType, time_t tTimeStamp)
{
	uint64_t nType = MAKELONGLONG(nMainType, nSubType);
	m_mTimeStamps[lUserId][nType] = tTimeStamp;

	saveTimeStamp(lUserId,nMainType,nSubType,tTimeStamp);
}

time_t TimeStampManager::GetTimeStamp(MPGUID lUserId, uint32_t nMainType, uint32_t nSubType)const
{
	if (auto itUser = m_mTimeStamps.find(lUserId); itUser != m_mTimeStamps.end())
	{
		uint64_t nType = MAKELONGLONG(nMainType, nSubType);
		if (auto itTimeStamp = itUser->second.find(nType); itTimeStamp != itUser->second.end())
		{
			return itTimeStamp->second;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void TimeStampManager::saveTimeStamp(MPGUID lUserId, uint32_t nMainType, uint32_t nSubType, time_t tTimeStamp)
{
	DBGameServer::TimeStamp	db_filter_ts,db_data_ts;
	db_filter_ts.set_id(lUserId);
	db_filter_ts.set_main_type(nMainType);
	db_filter_ts.set_sub_type(nSubType);

	db_data_ts.set_id(lUserId);
	db_data_ts.set_main_type(nMainType);
	db_data_ts.set_sub_type(nSubType);
	db_data_ts.set_time(tTimeStamp);

	SAVE2DB(db_filter_ts,db_data_ts);
}

void TimeStampManager::loadTimeStamp()
{
	m_mTimeStamps.clear();

	DBGameServer::TimeStamp filter;
	std::vector<DBGameServer::TimeStamp> vResults;

	g_pDBMgr->ExecSelectAll(filter, vResults);


	for (auto& ts : vResults)
	{
		uint64_t nType = MAKELONGLONG(ts.main_type(), ts.sub_type());
		m_mTimeStamps[ts.id()][nType] = ts.time();
	}
}