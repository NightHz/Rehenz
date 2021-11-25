// HRand.h
//
// By NightHz
//
// 伪随机数发生器
// 哈希值计算
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

		static const unsigned int max = (1U << 31) - 1U;
	};

	unsigned int Hash(void* pObj, unsigned int size);
	unsigned int Hash(unsigned int obj);
	unsigned int Hash(int obj);
}

#endif // !_CRAND_H_
