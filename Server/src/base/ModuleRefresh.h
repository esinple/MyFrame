#pragma once
#include <stdint.h>
#include "MPTime.h"

class ModuleRefresh
{
public:
	ModuleRefresh();
	~ModuleRefresh();
public:
	virtual void InitRefresh();
private:
	meplay::MPTime m_LastRefreshTime;
};