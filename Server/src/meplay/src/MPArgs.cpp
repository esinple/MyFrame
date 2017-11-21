#include "MPArgs.h"

using namespace meplay;

MPArgs::MPArgs(int nArgc, char** pArgv)
{
	std::string sKey;
	std::string sValue;
	for (int i = 1; i < nArgc; ++i)
	{

		auto pArg = pArgv[i];
		if (*pArg == '-')
		{
			if (!sKey.empty() && !sValue.empty())
			{
				m_mArgs.emplace(sKey, sValue);
				sKey.clear();
				sValue.clear();
			}
			while (*(++pArg) == '-') {};
			sKey.append(pArg);
		}
		else
		{
			if (!sValue.empty())
			{
				sValue.append(" ");
			}
			sValue.append(pArg);
		}
	}
	if (!sKey.empty() && !sValue.empty())
	{
		m_mArgs.emplace(sKey, sValue);
	}
}

MPArgs::MPArgs(const char* p)
	: MPArgs(std::string(p))
{
}

MPArgs::MPArgs(const std::string& s)
{
	auto nStart = s.find_first_of('-');
	while (nStart != std::string::npos)
	{
		auto nEnd = s.find_first_of(' ', nStart);
		auto sKey = s.substr(nStart + 1, nEnd - nStart - 1);
		auto sValue = s.substr(nEnd + 1, s.find_first_of('-', nEnd + 1) - nEnd - 2);
		nStart = s.find_first_of('-', nEnd + 1);
		m_mArgs.emplace(sKey, sValue);
	}
}

MPArgs::~MPArgs()
{

}

const std::string& MPArgs::GetArg(const std::string& s)const
{
	return GetArg(s.c_str());
}

const std::string& MPArgs::GetArg(const char * p)const
{
	auto it = m_mArgs.find(p);
	if (it == m_mArgs.end())
	{
		return m_kNullStr;
	}
	return it->second;
}