// HRand.cpp
//
// By NightHz
//

#include "HRand.h"

const unsigned int mask = (1U << 31) - 1U;
const unsigned int multiplier = 16807U; // 7^5
const unsigned int addend = 0;

Rehenz::CRand::CRand(unsigned int seed)
{
	this->seed = seed;
}

unsigned int Rehenz::CRand::Next()
{
	seed = ((seed * multiplier + addend) & mask);
	return seed;
}

unsigned int Rehenz::Hash(void * pObj, unsigned int size)
{
	unsigned int * p = (unsigned int *)pObj;
	unsigned int seed = 0U;
	unsigned int i = 0;
	if (size % 4 != 0)
	{
		unsigned char * p2 = (unsigned char *)pObj;
		for (i = size / 4 * 4; i < size; i++)
			seed = (seed << 8) | p2[i];
	}
	size /= 4;
	for (i = 0; i < size; i++)
		seed ^= p[i];
	for (i = 0; i < 5; i++)
		seed = ((seed * multiplier + addend) & mask);
	return seed;
}

unsigned int Rehenz::Hash(unsigned int obj)
{
	for (int i = 0; i < 5; i++)
		obj = ((obj * multiplier + addend) & mask);
	return obj;
}

unsigned int Rehenz::Hash(int obj)
{
	return Hash((unsigned int)obj);
}

