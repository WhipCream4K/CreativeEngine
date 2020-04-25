#pragma once
#include <type_traits>

namespace dae
{
	template<typename T>
	inline typename std::enable_if<std::is_arithmetic_v<T>, T>::type Clamp(T value,T low,T hi)
	{
		//assert(!(hi < low));
		return (value < low) ? low : (hi < value) ? hi : value;
	}
}

