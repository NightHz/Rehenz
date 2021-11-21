#include "noise_gen.h"
#include "math.h"

namespace Rehenz
{
	const unsigned int mask = ~0U;
	const unsigned int multiplier = 16807U; // 7^5
	const unsigned int addend = 0;
	const unsigned int max = ~0U;

	unsigned int hash(unsigned int obj)
	{
		for (int i = 0; i < 5; i++)
			obj = obj * multiplier + addend;
		return obj;
	}

	float hashToFloat(unsigned int h)
	{
		return (static_cast<float>(h) / max * 2 - 1);
	}

	float* perlin1D(unsigned int seed, int x1, int x2, unsigned int block_size, float* noise)
	{
		sort(x1, x2);
		float* p = noise;
		float g1, g2;
		g1 = hashToFloat(hash(seed ^ hash(x1)));
		for (int x = x1; x < x2; x++)
		{
			g2 = hashToFloat(hash(seed ^ hash(x+1)));
			for (unsigned int i = 0; i < block_size; i++)
				*(p++) = fade(g1, g2, static_cast<float>(i) / block_size);
			g1 = g2;
		}
		return noise;
	}
}