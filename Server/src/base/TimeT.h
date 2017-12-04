#pragma once
#include <functional>
#include <memory>
#include "MPGUID.h"
#include "MPTime.h"

enum TIME_EVENT_TYPE : int8_t
{
	eTET_UNKNOWN = -1,
	eTET_ONCE = 0,
	eTET_SEC_INTERVAL = 1,
	//eTET_DAY = 1,
	//eTET_HOUR = 2,
};

#define INVALID_TIME_EVENT_ID 0

class TimeEventBase
{
public:
	MPGUID id;
	int8_t nEventType;
	uint32_t nInterval;
	meplay::MPTime time;
	bool bValid;
public:
	TimeEventBase() 
		: id(0),nEventType(eTET_UNKNOWN),nInterval(0),time(),bValid(true)
	{
	};
	virtual ~TimeEventBase() {};

	virtual void Call() = 0;

};

template<typename Function, typename Tuple, std::size_t... Index>
decltype(auto) invoke_impl(Function&& func, Tuple&& t, std::index_sequence<Index...>)
{
	return func(std::get<Index>(std::forward<Tuple>(t))...);
}

template<typename Function, typename Tuple>
decltype(auto) invoke_cb(Function&& func, Tuple&& t)
{
	constexpr auto size = std::tuple_size<typename std::decay_t<Tuple>>::value;
	return invoke_impl(std::forward<Function>(func), std::forward<Tuple>(t), std::make_index_sequence<size>{});
}


template<typename T,typename... Args>
class TimeEventT final : public TimeEventBase
{
public:
	TimeEventT()
	{ 
		//param = std::make_tuple(args...); 
	}
	virtual ~TimeEventT() {}

	virtual void Call()override
	{
		//std::invoke(func, param);
		invoke_cb(func, param);
	}

public:
	std::function<void(Args...)> func;
	std::tuple<Args...> param;
};

class TimeEventLambda final : public TimeEventBase
{
public:
	TimeEventLambda()
	{
		//param = std::make_tuple(args...); 
	}
	virtual ~TimeEventLambda() {}

	virtual void Call()override
	{
		func();
	}

public:
	std::function<void()> func;
};


typedef std::shared_ptr<TimeEventBase> TimeEventPtr;