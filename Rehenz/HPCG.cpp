// HPCG.cpp
//
// By NightHz
//

#include "HPCG.h"
#include "HRand.h"
#include "HMath.h"
#include <cmath>

using namespace Rehenz;

float PerlinRand(unsigned int seed, int x)
{
	unsigned int hash = Hash(seed ^ Hash(x));
	return (float)((double)hash / CRand::max * 2 - 1);
}

struct Vector2D
{
	float x;
	float y;
};

Vector2D PerlinRand(unsigned int seed, Point2D point)
{
	Vector2D v;
	unsigned int hash = seed;
	while (true)
	{
		hash = Hash(hash ^ Hash(point.x));
		v.x = (float)((double)hash / CRand::max * 2 - 1);
		hash = Hash(hash ^ Hash(point.y));
		v.y = (float)((double)hash / CRand::max * 2 - 1);
		float length2 = v.x*v.x + v.y*v.y;
		if (length2 > 1 || length2 == 0)
			continue;
		else
		{
			float length = sqrtf(length2);
			v.x /= length;
			v.y /= length;
			break;
		}
	}
	return v;
}

float Rehenz::Perlin(unsigned int seed, float x)
{
	int xi = (int)x;
	float xf = x - xi;
	float g1 = PerlinRand(seed, xi);
	float g2 = PerlinRand(seed, xi + 1);
	return Fade(g1, g2, xf);
}

float DotGridGradient(unsigned int seed, int ix, int iy, float x, float y)
{
	float dx = x - ix;
	float dy = y - iy;
	Vector2D v = PerlinRand(seed, Point2D(ix, iy));
	return (dx*v.x + dy*v.y);
}

float Rehenz::Perlin(unsigned int seed, float x, float y)
{
	int xi = (int)x;
	int yi = (int)y;
	float xf = x - xi;
	float yf = y - yi;
	float n0, n1, n2, n3;
	n0 = DotGridGradient(seed, xi, yi, x, y);
	n1 = DotGridGradient(seed, xi + 1, yi, x, y);
	n2 = Fade(n0, n1, xf);
	n0 = DotGridGradient(seed, xi, yi + 1, x, y);
	n1 = DotGridGradient(seed, xi + 1, yi + 1, x, y);
	n3 = Fade(n0, n1, xf);
	return Fade(n2, n3, yf);
}

float Rehenz::PerlinOctave(unsigned int seed, float x, int octaves, float persistence)
{
	int frequency = 1;
	float amplitude = 1;
	float total = 0;
	float maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
	for (int i = 0;i < octaves;i++)
	{
		seed = Hash(seed);
		total += Perlin(seed, x * frequency) * amplitude;
		maxValue += amplitude;
		frequency *= 2;
		amplitude *= persistence;
	}
	return total / maxValue;
}

float Rehenz::PerlinOctave(unsigned int seed, float x, float y, int octaves, float persistence)
{
	int frequency = 1;
	float amplitude = 1;
	float total = 0;
	float maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
	for (int i = 0;i < octaves;i++)
	{
		seed = Hash(seed);
		total += Perlin(seed, x * frequency, y * frequency) * amplitude;
		maxValue += amplitude;
		frequency *= 2;
		amplitude *= persistence;
	}
	return total / maxValue;
}

