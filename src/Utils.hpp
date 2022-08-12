#pragma once

#include <random>
#include <algorithm>

template<typename T>
inline T random(T first, T second)
{
	static std::random_device rd;
	static std::mt19937 rng(rd());

	if constexpr (std::is_integral_v<T>)
	{
		std::uniform_int_distribution<T> uni(first, second);
		return uni(rng);
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		std::uniform_real_distribution<T> uni(first, second);
		return uni(rng);
	}

	return 0;
}

static inline void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
}

static inline void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
}

static inline void trim(std::string &s)
{
	ltrim(s);
	rtrim(s);
}
