#include "MPGUID.h"
#include <ctime>
#include "MPTime.h"
#include <thread>
#include <atomic>

std::atomic<uint64_t> g_nSeedPart(0);
std::atomic<uint64_t> g_nLastTime(0);

MPGUID GenerateNewGUID(int nWorkId)
{
	while (1)
	{
		meplay::MPTime now;
		uint64_t msec = now.CurrentMSec();
		if (msec != g_nLastTime)
		{
			g_nLastTime = msec;
			g_nSeedPart = 0;
		}
		if (g_nSeedPart >= 128)
		{
			continue;
		}

		constexpr uint64_t nTimeBit = ((uint64_t)1 << 39) - 1;
		uint64_t nTimePart = (msec & nTimeBit) << 25;

		constexpr uint64_t nWorkBit = ((uint64_t)1 << 18) - 1;
		uint64_t nWorkPart = (nWorkId & nWorkBit) << 7;
		//--------39-----|----18-----|-----7------//
		return nTimePart | nWorkPart | g_nSeedPart++;
	}
}
