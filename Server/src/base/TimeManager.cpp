#include "TimeManager.h"
#include "CommDef.h"
#include <iostream>

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::Run()
{
	for (auto& pEvent : m_vWaitList)
	{
		m_mTimes.emplace(pEvent->time, pEvent->id);
	}
	m_vWaitList.clear();

	m_CurrentTime.Now();
	auto it = m_mTimes.begin();
	while (it != m_mTimes.end())
	{
		if (it->first > m_CurrentTime)
		{
			return;
		}

		processEvent(it->second);
		m_mTimes.erase(it++);
	}
}

void TimeManager::DelTimeEvent(uint64_t lEventId)
{
	auto it = m_mEvents.find(lEventId);
	if (it == m_mEvents.end())
	{
		return;
	}
	it->second->bValid = false;
}

uint64_t TimeManager::addTimeEvent(TimeEventPtr pEvent)
{
	if (!checkTime(pEvent))
	{
		return 0;
	}
	
	m_mTimes.emplace(pEvent->time, pEvent->id);
	m_mEvents.emplace(pEvent->id, pEvent);
	return pEvent->id;
}

void TimeManager::delTimeEvent()
{
}

bool TimeManager::checkTime(TimeEventPtr pEvent)const
{
	switch (pEvent->nEventType)
	{
	case eTET_ONCE:
	{
		if (m_CurrentTime <= pEvent->time)
		{
			//pEvent->Call();
			return false;
		}
		return true;
	}
	break;
	case eTET_SEC_INTERVAL:
	{
		/*if (m_CurrentTime <= pEvent->time)
		{
			pEvent->Call();
		}*/
		auto nGap = (m_CurrentTime.CurrentSec() - pEvent->time.CurrentSec()) / pEvent->nInterval;
		pEvent->time += pEvent->nInterval * (nGap + 1);
		return true;
	}
	break;
	default:
		return false;
	}
}

uint64_t TimeManager::generateId()const
{
	static uint32_t nSeed(0);
	return MAKELONGLONG(m_CurrentTime.CurrentSec(), ++nSeed);
}


void TimeManager::processEvent(uint64_t lEventId)
{
	auto it = m_mEvents.find(lEventId);
	if (it == m_mEvents.end())
	{
		return;
	}
	auto pEvent = it->second;
	if (!pEvent->bValid)
	{
		m_mEvents.erase(lEventId);
		return;
	}

	pEvent->Call();

	switch (pEvent->nEventType)
	{
	case eTET_ONCE:
	{
	}
	break;
	case eTET_SEC_INTERVAL:
	{
		auto nGap = (m_CurrentTime.CurrentSec() - pEvent->time.CurrentSec()) / pEvent->nInterval;
		pEvent->time += pEvent->nInterval * (nGap + 1);
	}
	break;
	default:
		break;
	}

	waitAddTimeEvent(pEvent);
}

void TimeManager::waitAddTimeEvent(TimeEventPtr pEvent)
{
	m_vWaitList.emplace_back(pEvent);
}