#pragma once
#include <random>
#include <limits>

template<typename T>
T RandBetween(T min, T max)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<T> distribution(min, max);
	return distribution(generator);
}

static int32_t Rand32()
{
	return RandBetween<int>(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}
static uint32_t	RandU32()
{
	return RandBetween<uint32_t>(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max());
}

static int64_t Rand64()
{
	return RandBetween<int64_t>(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());
}

static uint64_t RandU64()
{
	return RandBetween<uint64_t>(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());
}