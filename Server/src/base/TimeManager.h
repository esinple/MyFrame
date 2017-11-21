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
		meplay::MPTime time,
		T* pObj, void(T::*pCB)(Args...), Args... args)
	{
		auto pEvent = std::make_shared<TimeEventT<T, Args...>>();
		pEvent->id = generateId();
		pEvent->nEventType = nEventType;
		pEvent->time = time;
		pEvent->func = std::bind(pCB, pObj, static_cast<Args&>(args)...);
		pEvent->param = std::make_tuple(static_cast<Args&>(args)...);

		return addTimeEvent(pEvent);
	}

	template<typename T, typename... Args>
	uint64_t AddTimeEvent(
		uint8_t nEventType,
		uint8_t nHour,
		uint8_t nMin,
		T* pObj, void(T::*pCB)(Args...), Args... args)
	{
		meplay::MPTime time(nHour, nMin);
		return AddTimeEvent(nEventType, time, pObj, pCB, args...);
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
