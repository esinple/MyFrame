#pragma once

#include <map>
#include <vector>
#include "TimeT.h"

class TimeManager final
{
public:
	TimeManager();
	~TimeManager();
public:
	template<typename T, typename... Args>
	uint64_t AddTimeEvent(
		uint8_t nEventType,
		uint32_t nIntervalSec,
		meplay::MPTime time,
		T* pObj, void(T::*pCB)(Args...), Args... args)
	{
		auto pEvent = std::make_shared<TimeEventT<T, Args...>>();
		pEvent->id = generateId();
		pEvent->nEventType = nEventType;
		pEvent->nInterval = nIntervalSec;
		pEvent->time = time;
		pEvent->func = std::bind(pCB, pObj, static_cast<Args&>(args)...);
		pEvent->param = std::make_tuple(static_cast<Args&>(args)...);

		return addTimeEvent(pEvent);
	}

	uint64_t AddTimeEvent(
		uint8_t nEventType,
		uint32_t nIntervalSec,
		meplay::MPTime time,
		std::function<void()> f)
	{
		auto pEvent = std::make_shared<TimeEventLambda>();
		pEvent->id = generateId();
		pEvent->nEventType = nEventType;
		pEvent->nInterval = nIntervalSec;
		pEvent->time = time;
		pEvent->func = f;

		return addTimeEvent(pEvent);
	}

	template<typename T, typename... Args>
	uint64_t AddTimeEventInterval(
		uint32_t nIntervalSec,
		uint8_t nHour,
		uint8_t nMin,
		T* pObj, void(T::*pCB)(Args...), Args... args)
	{
		meplay::MPTime time(nHour, nMin);
		return AddTimeEvent(eTET_SEC_INTERVAL,nIntervalSec, time, pObj, pCB, args...);
	}

	template<typename T, typename... Args>
	uint64_t AddTimeEventOnce(
		uint8_t nHour,
		uint8_t nMin,
		T* pObj, void(T::*pCB)(Args...), Args... args)
	{
		meplay::MPTime time(nHour, nMin);
		return AddTimeEvent(eTET_ONCE, 0, time, pObj, pCB, args...);
	}

	void DelTimeEvent(uint64_t lEventId);
	void Run();
private:
	uint64_t generateId()const;

	uint64_t addTimeEvent(TimeEventPtr pEvent);
	void waitAddTimeEvent(TimeEventPtr pEvent);
	void delTimeEvent();
	void processEvent(uint64_t lEventId);
	bool checkTime(TimeEventPtr pEvent)const;
private:
	meplay::MPTime m_CurrentTime;
	std::multimap<meplay::MPTime, uint64_t> m_mTimes;
	std::unordered_map<uint64_t, TimeEventPtr> m_mEvents;
	std::vector<TimeEventPtr> m_vWaitList;
};
