// CRand.cpp
//
// By NightHz
//

#include "CRand.h"

const unsigned long mask = (1UL << 31) - 1UL;
const unsigned long multiplier = 16807UL; // 7^5
const unsigned long addend = 0;

Rehenz::CRand::CRand(unsigned int seed)
{
	this->seed = seed;
}

unsigned int Rehenz::CRand::Next()
{
	seed = (unsigned int)((seed * multiplier + addend) & mask);
	return seed;
}
