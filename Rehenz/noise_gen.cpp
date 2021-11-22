#include "noise_gen.h"
#include "math.h"
#include <random>

namespace Rehenz
{
	const unsigned int mask = ~0U;
	const unsigned int multiplier = 16807U; // 7^5
	const unsigned int addend = 0;

	unsigned int Hash(unsigned int obj)
	{
		for (int i = 0; i < 5; i++)
			obj = obj * multiplier + addend;
		return obj;
	}

	void GHash(unsigned int seed, int x, float& g)
	{
		std::default_random_engine e(Hash(seed ^ Hash(x)));
		std::uniform_real_distribution<float> d(-1, 1);
		g = d(e);
	}

	float* Perlin1D(unsigned int seed, int x1, int x2, unsigned int block_size, float* noise)
	{
		Sort(x1, x2);
		float* p = noise;
		float g1, g2;
		GHash(seed, x1, g1);
		for (int x = x1; x < x2; x++)
		{
			GHash(seed, x + 1, g2);
			for (unsigned int i = 0; i < block_size; i++)
				*(p++) = Fade(g1, g2, static_cast<float>(i) / block_size);
			g1 = g2;
		}
		return noise;
	}
}