#include "ModuleTimeTick.h"

ModuleTimeTick::ModuleTimeTick()
{
}

ModuleTimeTick::~ModuleTimeTick()
{
}

const meplay::MPTime& ModuleTimeTick::GetLastTick()const
{
	return m_LastTick;
}

bool ModuleTimeTick::Tick(const meplay::MPTime& now, std::chrono::milliseconds msec)
{
	if (now.CurrentMSec() >= m_LastTick.CurrentMSec() + msec.count())
	{
		m_LastTick = now;
		return true;
	}

	return false;
}

bool ModuleTimeTick::Tick(const meplay::MPTime& now, std::chrono::seconds sec)
{
	if (now.CurrentSec() >= m_LastTick.CurrentSec() + sec.count())
	{
		m_LastTick = now;
		return true;
	}

	return false;
}

bool ModuleTimeTick::Tick(const meplay::MPTime& now, std::chrono::minutes min)
{
	if (now.CurrentSec() >= m_LastTick.CurrentSec() + min.count() * 60)
	{
		m_LastTick = now;
		return true;
	}

	return false;
}

bool ModuleTimeTick::Tick(const meplay::MPTime& now, std::chrono::hours hour)
{
	if (now.CurrentSec() >= m_LastTick.CurrentSec() + hour.count() * 60 * 60)
	{
		m_LastTick = now;
		return true;
	}

	return false;
}