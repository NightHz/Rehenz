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

	// 【弃用】因为浮点数计算的不精确，长度可能不是预期值 PerlinNoise1D 获得[x1,x2)区间的噪点值 pValue需指向一块足够大的空间
	void Perlin(unsigned int seed, float x1, float x2, float xStep, float *pValue);

	// PerlinNoise2D
	float Perlin(unsigned int seed, float x, float y);

	// 【弃用】因为浮点数计算的不精确，长度可能不是预期值 PerlinNoise2D 获得[x1,x2)*[y1,y2)区间的噪点值 pValue需指向一块足够大的空间 pValue按行存储噪点
	void Perlin(unsigned int seed, float x1, float x2, float xStep, float y1, float y2, float yStep, float *pValue);

	float PerlinOctave(unsigned int seed, float x, int octaves, float persistence);

	float PerlinOctave(unsigned int seed, float x, float y, int octaves, float persistence);


}

#endif // !_HPCG_H_
