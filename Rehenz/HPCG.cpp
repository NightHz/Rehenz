// HPCG.cpp
//
// By NightHz
//

#include "HPCG.h"
#include "HRand.h"
#include "HMath.h"

using namespace Rehenz;

float PerlinRand(unsigned int seed, int x)
{
	return (float)((double)Hash(seed ^ Hash(x)) / CRand::max * 2 - 1);
}

float PerlinRand(unsigned int seed, int x, int y)
{
	return (float)((double)Hash(seed ^ Hash(x) ^ Hash(Hash(y))) / CRand::max * 2 - 1);
}

float Rehenz::Perlin(unsigned int seed, float x)
{
	int xi = (int)x;
	float xf = x - xi;
	float g1 = PerlinRand(seed, xi);
	float g2 = PerlinRand(seed, xi + 1);
	return Fade(g1, g2, xf);
}

float Rehenz::Perlin(unsigned int seed, float x, float y)
{
	int xi = (int)x;
	int yi = (int)y;
	float xf = x - xi;
	float yf = y - yi;
	float g1 = PerlinRand(seed, xi, yi);
	float g2 = PerlinRand(seed, xi + 1, yi);
	float g3 = PerlinRand(seed, xi, yi + 1);
	float g4 = PerlinRand(seed, xi + 1, yi + 1);
	return Fade(Fade(g1, g2, xf), Fade(g3, g4, xf), yf);
}

float Rehenz::Perlin(unsigned int seed, float x, float y, float z)
{
	return 0.0f;
}
