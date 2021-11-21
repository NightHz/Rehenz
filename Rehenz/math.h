#pragma once

namespace Rehenz
{
	const float pi = 3.14159265359f;

	// limit x to [min, max]
	template <typename T>
	inline T clamp(T x, T min, T max)
	{
		return (x < min) ? min : ((x > max) ? max : x);
	}

	// interpolation
	template <typename T>
	inline T lerp(T x1, T x2, float t)
	{
		return x1 + (x2 - x1) * t;
	}

	// f(0)=0, f(1)=1, f(t) = 6*t^5 - 15*t^4 + 10*t^3
	inline float fade(float t)
	{
		return t * t * t * (10 + t * (-15 + t * 6));
	}

	// smooth interpolation
	template <typename T>
	inline T fade(T x1, T x2, float t)
	{
		return x1 + (x2 - x1) * fade(t);
	}

	// sort two number, first <= second
	template <typename T>
	inline void sort(T& x1, T& x2)
	{
		if (x1 > x2)
		{
			T t = x1;
			x1 = x2;
			x2 = t;
		}
	}
}