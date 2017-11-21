#pragma once
#include <chrono>
#include <string>
#include <ctime>
#include <unordered_map>

namespace meplay
{
	
	class MPTime final
	{
	public:
		MPTime(std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now());
		MPTime(uint8_t hour, uint8_t min = 0, uint8_t sec = 0);
		MPTime(uint16_t year, uint8_t month, uint8_t day,uint8_t hour, uint8_t min, uint8_t sec, int8_t dst = -1);
		MPTime(const MPTime&);
		MPTime(MPTime&&);
		~MPTime();
	public:
		bool operator<(const MPTime&)const;
		bool operator>(const MPTime&)const;
		bool operator<=(const MPTime&)const;
		bool operator>=(const MPTime&)const;
		bool operator==(const MPTime&)const;
		bool operator!=(const MPTime&)const;

		MPTime operator+(time_t sec)const;
		MPTime& operator+=(time_t sec);
		MPTime operator-(time_t sec)const;
		MPTime& operator-=(time_t sec);

		MPTime& operator=(const MPTime&);

	public:
		void Now();
		time_t CurrentSec()const;
		time_t CurrentMSec()const;

		tm ToTm()const;
		
		std::string ToCTime()const;
		void ToCTime(std::string&)const;

		time_t MTimeOut(time_t millisecond)const;
		time_t TimeOut(time_t second)const;
		bool IsMTimeOut(time_t millisecond)const;
		bool IsTimeOut(time_t second)const;
	public:
		std::chrono::time_point<std::chrono::system_clock> m_timepoint;
	};
}

namespace std {
#ifdef WIN_SYSTEM
	template<> struct std::hash<meplay::MPTime>
	{
		size_t operator()(const meplay::MPTime& t)const
		{
			return std::hash<uint64_t>{}(t.CurrentMSec());
		}
	};
#else
	template<> 
        struct hash<meplay::MPTime> : public __hash_base<size_t,meplay::MPTime>
        {
            size_t operator()(const meplay::MPTime& t)const noexcept
            {
                return hash<uint64_t>{}(t.CurrentMSec());
            }
        };
#endif
}
