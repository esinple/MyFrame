#pragma once
#include "MPTime.h"

class ModuleTimeTick
{
public:
	ModuleTimeTick();
	~ModuleTimeTick();
public:
	const meplay::MPTime& GetLastTick()const;

	bool Tick(const meplay::MPTime& now,std::chrono::milliseconds msec);
	bool Tick(const meplay::MPTime& now,std::chrono::seconds sec);
	bool Tick(const meplay::MPTime& now,std::chrono::minutes min);
	bool Tick(const meplay::MPTime& now,std::chrono::hours hours);
private:
	meplay::MPTime m_LastTick;
};