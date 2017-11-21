#pragma once
#include <map>
#include <string>

namespace meplay {
	class MPArgs final
	{
	public:
		MPArgs(int nArgc, char** pArgv);
		MPArgs(const char* p);
		MPArgs(const std::string& s);
		~MPArgs();
	public:
		const std::string& GetArg(const std::string& s)const;
		const std::string& GetArg(const char * p)const;
	private:
		std::map<std::string, std::string> m_mArgs;
		const std::string m_kNullStr;
	};
}