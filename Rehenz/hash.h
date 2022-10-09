#pragma once
#include <functional>
#include "math.h"

// std::hash for Matrix
template<>
struct std::hash<Rehenz::Matrix>
{
	std::size_t operator()(Rehenz::Matrix const& m) const noexcept
	{
		std::size_t h = 0;
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				h = std::hash<float>{}(m(x, y)) ^ (h << 1);
		return h;
	}
};

// std::hash for Vector
template<>
struct std::hash<Rehenz::Vector>
{
	std::size_t operator()(Rehenz::Vector const& v) const noexcept
	{
		std::size_t h = 0;
		for (int i = 0; i < 4; i++)
			h = std::hash<float>{}(v(i)) ^ (h << 1);
		return h;
	}
};

// std::hash for Point
template<>
struct std::hash<Rehenz::Point>
{
	std::size_t operator()(Rehenz::Point const& p) const noexcept
	{
		std::size_t h = 0;
		for (int i = 0; i < 4; i++)
			h = std::hash<float>{}(p(i)) ^ (h << 1);
		return h;
	}
};

// std::hash for Quaternion
template<>
struct std::hash<Rehenz::Quaternion>
{
	std::size_t operator()(Rehenz::Quaternion const& q) const noexcept
	{
		std::size_t h = 0;
		for (int i = 0; i < 4; i++)
			h = std::hash<float>{}(q(i)) ^ (h << 1);
		return h;
	}
};

// std::hash for EulerAngles
template<>
struct std::hash<Rehenz::EulerAngles>
{
	std::size_t operator()(Rehenz::EulerAngles const& ea) const noexcept
	{
		std::size_t h1 = std::hash<float>{}(ea.psi);
		std::size_t h2 = std::hash<float>{}(ea.theta);
		std::size_t h3 = std::hash<float>{}(ea.phi);
		return (((h1 << 1) ^ h2) << 1) ^ h3;
	}
};

// std::hash for AircraftAxes
template<>
struct std::hash<Rehenz::AircraftAxes>
{
	std::size_t operator()(Rehenz::AircraftAxes const& aa) const noexcept
	{
		std::size_t h1 = std::hash<float>{}(aa.pitch);
		std::size_t h2 = std::hash<float>{}(aa.yaw);
		std::size_t h3 = std::hash<float>{}(aa.roll);
		return (((h1 << 1) ^ h2) << 1) ^ h3;
	}
};

// std::hash for Vector2
template<>
struct std::hash<Rehenz::Vector2>
{
	std::size_t operator()(Rehenz::Vector2 const& v) const noexcept
	{
		std::size_t h1 = std::hash<float>{}(v.x);
		std::size_t h2 = std::hash<float>{}(v.y);
		return (h1 << 1) ^ h2;
	}
};

// std::hash for Vector3
template<>
struct std::hash<Rehenz::Vector3>
{
	std::size_t operator()(Rehenz::Vector3 const& v) const noexcept
	{
		std::size_t h1 = std::hash<float>{}(v.x);
		std::size_t h2 = std::hash<float>{}(v.y);
		std::size_t h3 = std::hash<float>{}(v.z);
		return (((h1 << 1) ^ h2) << 1) ^ h3;
	}
};

// std::hash for Vector2I
template<>
struct std::hash<Rehenz::Vector2I>
{
	std::size_t operator()(Rehenz::Vector2I const& v) const noexcept
	{
		std::size_t h1 = std::hash<int>{}(v.x);
		std::size_t h2 = std::hash<int>{}(v.y);
		return (h1 << 1) ^ h2;
	}
};
