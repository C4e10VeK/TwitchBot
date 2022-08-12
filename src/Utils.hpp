#pragma once

#include <random>

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
