#pragma once
#include <ctime>
#include <chrono>
#include <string>

namespace meplay {
	class MPTimeTester final
	{
	public:
		MPTimeTester(const std::string& name,uint64_t nMinDelay = 0);
		~MPTimeTester();
	public:
		void Show(uint64_t nMinDelay = 0)const;
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_timepoint;
		std::string m_sName;
		uint64_t m_nMinDelay;
	};
}
