#pragma once

namespace Rehenz
{
	// generate 1D perlin noise, return noise ptr
	float* perlin1D(unsigned int seed, int x1, int x2, unsigned int block_size, float* noise);
}