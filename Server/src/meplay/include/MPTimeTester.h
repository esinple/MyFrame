#pragma once
#include <ctime>
#include <chrono>
#include <string>
#include <string_view>

namespace meplay {
	class MPTimeTester final
	{
	public:
		MPTimeTester(std::string_view name,uint64_t nMinDelay = 0);
		~MPTimeTester();
	public:
		void Show(uint64_t nMinDelay = 0)const;
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_timepoint;
		std::string_view m_sName;
		uint64_t m_nMinDelay;
	};
}
