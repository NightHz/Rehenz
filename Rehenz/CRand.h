// CRand.h
//
// By NightHz
//
// 伪随机数发生器
//

#ifndef _CRAND_H_
#define _CRAND_H_

namespace Rehenz
{
	// 0 ~ 2^31-1
	class CRand
	{
	private:
		unsigned int seed;

	public:
		CRand(unsigned int seed);

		unsigned int Next();
	};
}

#endif // !_CRAND_H_
