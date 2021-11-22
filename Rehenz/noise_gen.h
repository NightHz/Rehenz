#pragma once
#include <map>
#include "math.h"

namespace Rehenz
{
	// generate 1D perlin noise, return noise ptr
	float* Perlin1D(unsigned int seed, int x1, int x2, unsigned int block_size, float* noise);

	// 1-dim Perlin noise
	class PerlinNoise1D
	{
	private:
		unsigned int seed;
		std::map<int, float> grads;
		float GetGrad(int x);
	public:
		PerlinNoise1D(unsigned int _seed);
		~PerlinNoise1D();
		float GetNoise(float x);
	};
}