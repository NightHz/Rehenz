#include "noise_gen.h"
#include "math.h"
#include <random>

namespace Rehenz
{
	const unsigned int mask = ~0U;
	const unsigned int multiplier = 16807U; // 7^5
	const unsigned int addend = 0;

	unsigned int hash(unsigned int obj)
	{
		for (int i = 0; i < 5; i++)
			obj = obj * multiplier + addend;
		return obj;
	}

	void ghash(unsigned int seed, int x, float& g)
	{
		std::default_random_engine e(hash(seed ^ hash(x)));
		std::uniform_real_distribution<float> d(-1, 1);
		g = d(e);
	}

	float* perlin1D(unsigned int seed, int x1, int x2, unsigned int block_size, float* noise)
	{
		sort(x1, x2);
		float* p = noise;
		float g1, g2;
		ghash(seed, x1, g1);
		for (int x = x1; x < x2; x++)
		{
			ghash(seed, x + 1, g2);
			for (unsigned int i = 0; i < block_size; i++)
				*(p++) = fade(g1, g2, static_cast<float>(i) / block_size);
			g1 = g2;
		}
		return noise;
	}
}