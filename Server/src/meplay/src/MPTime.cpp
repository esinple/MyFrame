#include "MPTime.h"

using namespace meplay;
using namespace std::chrono;

MPTime::MPTime(std::chrono::time_point<std::chrono::system_clock> time)
	: m_timepoint(time)
{
}

MPTime::MPTime(time_t timestamp)
{
	m_timepoint = system_clock::from_time_t(timestamp);
}

MPTime::MPTime(uint8_t hour, uint8_t min, uint8_t sec)
	: m_timepoint(system_clock::now())
{
	auto t = ToTm();
	t.tm_hour = hour;
	t.tm_min = min;
	t.tm_sec = sec;
	m_timepoint = system_clock::from_time_t(mktime(&t));
}

MPTime::MPTime(uint16_t year, uint8_t month, uint8_t day,uint8_t hour, uint8_t min, uint8_t sec, int8_t dst)
{
	std::tm t;
	t.tm_year = year - 1900;
	t.tm_mon = month - 1;
	t.tm_mday = day;
	t.tm_hour = hour;
	t.tm_min = min;
	t.tm_sec = sec;
	t.tm_isdst = dst;
	m_timepoint = system_clock::from_time_t(mktime(&t));
}

MPTime::MPTime(const MPTime& rhs) 
	: m_timepoint(std::move(rhs.m_timepoint))
{
}

MPTime::MPTime(MPTime&& rhs)
	: m_timepoint(std::move(rhs.m_timepoint))
{
}

MPTime::~MPTime()
{
}

bool MPTime::operator<(const MPTime& rhs)const
{
	return m_timepoint < rhs.m_timepoint;
}
bool MPTime::operator>(const MPTime& rhs)const
{
	return m_timepoint > rhs.m_timepoint;
}

bool MPTime::operator<=(const MPTime& rhs)const
{
	return m_timepoint <= rhs.m_timepoint;
}

bool MPTime::operator>=(const MPTime& rhs)const
{
	return m_timepoint >= rhs.m_timepoint;
}

bool MPTime::operator==(const MPTime& rhs)const
{
	return m_timepoint == rhs.m_timepoint;
}

bool MPTime::operator!=(const MPTime& rhs)const
{
	return m_timepoint != rhs.m_timepoint;
}

MPTime MPTime::operator+(const time_t sec)const
{
	return m_timepoint + seconds(sec);
}

MPTime& MPTime::operator+=(const time_t sec)
{
	m_timepoint += seconds(sec);
	return *this;
}

MPTime MPTime::operator-(const time_t sec)const
{
	return m_timepoint - seconds(sec);
}

MPTime& MPTime::operator-=(const time_t sec)
{
	m_timepoint -= seconds(sec);
	return *this;
}
MPTime& MPTime::operator=(const MPTime& rhs)
{
	m_timepoint = rhs.m_timepoint;
	return *this;
}

void MPTime::Now()
{
	m_timepoint = system_clock::now();
}

time_t MPTime::CurrentSec()const
{
	return duration_cast<seconds>(m_timepoint.time_since_epoch()).count();
}

time_t MPTime::CurrentMSec()const
{
	return duration_cast<milliseconds>(m_timepoint.time_since_epoch()).count();
}

tm MPTime::ToTm()const
{
	time_t now = CurrentSec();
	return *(std::localtime(&now));
}

std::string MPTime::ToCTime()const
{
	std::string s;
	ToCTime(s);
	return std::move(s);
}
void MPTime::ToCTime(std::string& s)const
{
	auto tNow = CurrentSec();
	s = ctime(&tNow);
}

time_t MPTime::MTimeOut(time_t millisecond)const
{
	MPTime now;
	auto timeout = now.CurrentMSec() - CurrentMSec();
	/*if (timeout >= millisecond)
	{
		return 0;
	}*/
	return millisecond - timeout;
}
time_t MPTime::TimeOut(time_t second)const
{
	MPTime now;
	auto timeout = now.CurrentSec() - CurrentSec();
	/*if (timeout >= second)
	{
		return 0;
	}*/
	return second - timeout;
}

bool MPTime::IsMTimeOut(time_t millisecond)const
{
	MPTime now;
	return now.CurrentMSec() - CurrentMSec() > millisecond;
}
bool MPTime::IsTimeOut(time_t second)const
{
	MPTime now;
	return now.CurrentSec() - CurrentSec() > second;
}
