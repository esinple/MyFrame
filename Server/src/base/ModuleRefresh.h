#pragma once
#include <stdint.h>
#include <functional>
#include "MPTime.h"

class TimeManager;
class ModuleRefresh
{
public:
	explicit ModuleRefresh(TimeManager* pTimeMgr);
	~ModuleRefresh();
public:
	virtual void InitRefresh() = 0;
public:
	void RegisterCallBack(uint8_t nHour,uint8_t nMin,uint32_t nIntervalSec,std::function<void()> f);
private:
	meplay::MPTime m_LastRefreshTime;
};