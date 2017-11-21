#pragma once
#include "MPTime.h"

class TimeTick
{
public:	
	TimeTick();
	virtual ~TimeTick();
public:
	virtual void Tick() = 0;
private:
	meplay::MPTime m_CurTime;
};