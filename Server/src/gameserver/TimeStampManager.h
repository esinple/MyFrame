#pragma once
#include "GameCommDef.h"
#include <unordered_map>

enum eTS_MainType : uint32_t
{
	eTS_UserModuleRefresh	= 100001,
};

enum eTS_SubType : uint32_t
{
};

#define TS_GLOBAL_ID 0 

class TimeStampManager : public ManagerModule
{
public:
	TimeStampManager();
	~TimeStampManager();
public:
	virtual bool Awake()override;
	virtual bool AfterAwake()override;
	virtual bool Execute()override;
	virtual bool BeforeShutDown()override;
	virtual bool ShutDown()override;
public:
	void UpdateTimeStamp(MPGUID lUserId,uint32_t nMainType,uint32_t nSubType,time_t tTimeStamp);
	time_t GetTimeStamp(MPGUID lUserId, uint32_t nMainType, uint32_t nSubType)const;
private:
	void saveTimeStamp(MPGUID lUserId, uint32_t nMainType, uint32_t nSubType, time_t tTimeStamp);
	void loadTimeStamp();
private:
	typedef std::unordered_map<uint64_t, time_t> TIME_STAMP_MAP;
	std::unordered_map<uint64_t, TIME_STAMP_MAP> m_mTimeStamps;
};

GAME_MANAGER_MODULE_REG(eGameMgr_TimeStamp, TimeStampManager);