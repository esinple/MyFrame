#pragma once

#include <vector>
#include <string>

class LogMgr
{
public:
	static LogMgr* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new LogMgr();
		}
		return m_pInstance;
	}
private:
	LogMgr();
	~LogMgr();
public:
	void AddLog(std::string& sLog);
private:
	static LogMgr* m_pInstance;
};