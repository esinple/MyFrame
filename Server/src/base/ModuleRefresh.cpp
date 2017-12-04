#include "ModuleRefresh.h"
#include "TimeManager.h"

ModuleRefresh::ModuleRefresh(TimeManager* pTimeMgr)
{
}

ModuleRefresh::~ModuleRefresh()
{
}

void ModuleRefresh::RegisterCallBack(uint8_t nHour, uint8_t nMin, uint32_t nIntervalSec, std::function<void()> f)
{
}
