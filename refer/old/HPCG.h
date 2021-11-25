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

	// �����á���Ϊ����������Ĳ���ȷ�����ȿ��ܲ���Ԥ��ֵ PerlinNoise1D ���[x1,x2)��������ֵ pValue��ָ��һ���㹻��Ŀռ�
	void Perlin(unsigned int seed, float x1, float x2, float xStep, float *pValue);

	// PerlinNoise2D
	float Perlin(unsigned int seed, float x, float y);

	// �����á���Ϊ����������Ĳ���ȷ�����ȿ��ܲ���Ԥ��ֵ PerlinNoise2D ���[x1,x2)*[y1,y2)��������ֵ pValue��ָ��һ���㹻��Ŀռ� pValue���д洢���
	void Perlin(unsigned int seed, float x1, float x2, float xStep, float y1, float y2, float yStep, float *pValue);

	float PerlinOctave(unsigned int seed, float x, int octaves, float persistence);

	float PerlinOctave(unsigned int seed, float x, float y, int octaves, float persistence);


}

#endif // !_HPCG_H_
