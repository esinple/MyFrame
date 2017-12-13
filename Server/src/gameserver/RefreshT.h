#pragma once
#include <stdint.h>
#include <vector>
#include <tuple>
#include <any>

enum eIntervalType
{
	eIT_Seconds					= 1, //根据秒数
	eIT_Mins					= 2, //根据分钟数
	eIT_Hours					= 3, //根据小时数
	eIT_Days					= 4, //根据天数
	eIT_Special_Week_Day		= 5, //特定周几刷新
};

struct UserRefreshCfg
{
	uint64_t nId;
	uint32_t nRefreshModule;
	std::vector<std::tuple<uint8_t, uint8_t>> vRefreshTime;
	uint8_t nIntervalType;
	uint32_t nIntervalParam;
	std::any param;

	//calc
	std::vector<std::tuple<meplay::MPTime,meplay::MPTime>> vCalcTime;
	uint32_t nIntervalSec;
};


