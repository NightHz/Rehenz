// HMath.h
//
// By NightHz
//
// Math library
//

#ifndef _HMATH_H_
#define _HMATH_H

namespace Rehenz
{
	const float pi = 3.14159265f;

	// ����x��min��max֮��
	template <typename T>
	inline T Clamp(T x, T min, T max)
	{
		return (x < min) ? min : ((x > max) ? max : x);
	}

	// ���Բ�ֵ
	template <typename T>
	inline T Lerp(T x1, T x2, float t)
	{
		return x1 + (T)((x2 - x1)*t);
	}

	// ��������
	inline float Fade(float t)
	{
		return t * t * t * (10 + t * (-15 + t * 6)); // 6*t^5 - 15*t^4 + 10*t^3
	}

	// ������ƽ����ֵ ����������ֵ
	template <typename T>
	inline T Fade(T x1, T x2, float t)
	{
		return x1 + (T)((x2 - x1)*Fade(t));
	}
}

#endif // !_HMATH_H_
