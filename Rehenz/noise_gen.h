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

	struct Point2Compare
	{
		bool operator() (const Point2& lhs, const Point2& rhs) const;
	};

	// 2-dim Perlin noise
	class PerlinNoise2D
	{
	private:
		unsigned int seed;
		std::map<Point2, Vector2, Point2Compare> grads;
		Vector2 GetGrad(int x, int y);
	public:
		PerlinNoise2D(unsigned int _seed);
		~PerlinNoise2D();
		float GetNoise(float x, float y);
	};

	struct Point3Compare
	{
		bool operator() (const Point3& lhs, const Point3& rhs) const;
	};

	// 3-dim Perlin noise
	class PerlinNoise3D
	{
	private:
		unsigned int seed;
		std::map<Point3, Vector3, Point3Compare> grads;
		Vector3 GetGrad(int x, int y, int z);
	public:
		PerlinNoise3D(unsigned int _seed);
		~PerlinNoise3D();
		float GetNoise(float x, float y, float z);
	};
}