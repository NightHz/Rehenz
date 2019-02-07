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

	float PerlinOctave(unsigned int seed, float x, int octaves, float persistence);

	float PerlinOctave(unsigned int seed, float x, float y, int octaves, float persistence);

}

#endif // !_HPCG_H_
