#include "MPTimeTester.h"
#include "MPLogger.h"

using namespace meplay;

MPTimeTester::MPTimeTester(std::string_view name,uint64_t nMinDelay)
	: m_timepoint(std::chrono::steady_clock::now()),m_sName(name),m_nMinDelay(nMinDelay)
{
}

MPTimeTester::~MPTimeTester()
{
	Show(m_nMinDelay);
}


void MPTimeTester::Show(uint64_t nMinDelay)const
{
	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	auto nDelay = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - m_timepoint).count();
	if (nDelay > nMinDelay)
	{
		MP_WARN("[%s] used %d milliseconds!", m_sName.data(), nDelay);
	}
}