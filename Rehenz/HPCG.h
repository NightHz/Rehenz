// HPCG.h
//
// By NightHz
//
// Procedural content generation
//

#ifndef _HPCG_H_
#define _HPCG_H_

namespace Rehenz
{
	// PerlinNoise1D
	float Perlin(unsigned int seed, float x);

	// PerlinNoise2D
	float Perlin(unsigned int seed, float x, float y);

	// PerlinNoise3D
	float Perlin(unsigned int seed, float x, float y, float z);

}

#endif // !_HPCG_H_
