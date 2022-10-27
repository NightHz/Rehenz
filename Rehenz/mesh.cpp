#include "mesh.h"
#include "util.h"
#include <fstream>
#include <sstream>

namespace Rehenz
{
	Vertex::Vertex() : p(0, 0, 0), n(0, 0, 0), c(1, 1, 1), uv(0, 0), uv2(0, 0), coef(1)
	{
	}

	Vertex::Vertex(Point _p) : p(_p), n(0, 0, 0), c(1, 1, 1), uv(0, 0), uv2(0, 0), coef(1)
	{
	}

	Vertex::Vertex(Point _p, Vector _n, Color _c, UV _uv, UV _uv2, float _coef)
		: p(_p), n(_n), c(_c), uv(_uv), uv2(_uv2), coef(_coef)
	{
	}

	Vertex::Vertex(Point _p, Color _c, UV _uv, UV _uv2, float _coef)
		: p(_p), n(0, 0, 0), c(_c), uv(_uv), uv2(_uv2), coef(_coef)
	{
	}

	Vertex::Vertex(Point _p, UV _uv, UV _uv2, float _coef)
		: p(_p), n(0, 0, 0), c(1, 1, 1), uv(_uv), uv2(_uv2), coef(_coef)
	{
	}

	Vertex& Vertex::operator*=(float f)
	{
		p *= f;
		n *= f;
		c *= f;
		uv *= f;
		uv2 *= f;
		coef *= f;
		return *this;
	}

	Vertex& Vertex::operator+=(const Vertex& v)
	{
		p += v.p;
		n += v.n;
		c += v.c;
		uv += v.uv;
		uv2 += v.uv2;
		coef += v.coef;
		return *this;
	}

	Vertex Vertex::operator*(float f) const
	{
		return Vertex(p * f, n * f, c * f, uv * f, uv2 * f, coef * f);
	}

	Vertex Vertex::operator+(const Vertex& v) const
	{
		return Vertex(p + v.p, n + v.n, c + v.c, uv + v.uv, uv2 + v.uv2, coef + v.coef);
	}

	Vertex Vertex::operator-(const Vertex& v) const
	{
		return Vertex(p - v.p, n - v.n, c - v.c, uv - v.uv, uv2 - v.uv2, coef - v.coef);
	}

	Vertex VertexRecover(const Vertex& v)
	{
		float f = 1 / v.coef;
		return Vertex(v.p, v.n * f, v.c * f, v.uv * f, v.uv2 * f, 1);
	}

	Vertex VertexLerp(const Vertex& v1, const Vertex& v2, float t)
	{
		return v1 + (v2 - v1) * t;
	}



	Mesh::Mesh() : vertices(), triangles()
	{
	}
	Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<int>& _triangles) : vertices(_vertices), triangles(_triangles)
	{
	}
	Mesh::Mesh(const std::vector<Vertex>&& _vertices, const std::vector<int>&& _triangles) : vertices(_vertices), triangles(_triangles)
	{
	}
	Mesh::~Mesh()
	{
	}
	void Mesh::AddVertex(Vertex vertex)
	{
		vertices.push_back(vertex);
	}
	void Mesh::AddVertex(const std::vector<Vertex>& _vertices)
	{
		vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
	}
	void Mesh::AddTriangle(int a, int b, int c)
	{
		triangles.push_back(a);
		triangles.push_back(b);
		triangles.push_back(c);
	}
	void Mesh::AddTriangle(const std::vector<int>& _triangles)
	{
		triangles.insert(triangles.end(), _triangles.begin(), _triangles.end());
	}



	std::shared_ptr<Mesh> CreateCubeMesh(const std::vector<Color>& colors, int smooth)
	{
		smooth = Clamp(smooth, 1, 200);

		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		auto GetColor = [&colors](int i) -> Color
		{
			if (colors.size() == 0)
				return Color(1, 1, 1);
			else
				return colors[i % colors.size()];
		};
		auto SmoothFace = [&vertices, &triangles, smooth]()
		{
			size_t base_vi = vertices.size() - 4;
			Vertex v0 = vertices[base_vi + 0];
			Vertex v1 = vertices[base_vi + 1];
			Vertex v2 = vertices[base_vi + 2];
			Vertex v3 = vertices[base_vi + 3];
			vertices.resize(base_vi);
			// add vertices
			for (int y = 0; y <= smooth; y++)
			{
				Vertex v02 = VertexLerp(v0, v2, static_cast<float>(y) / smooth);
				Vertex v12 = VertexLerp(v1, v2, static_cast<float>(y) / smooth);
				Vertex v13 = VertexLerp(v1, v3, static_cast<float>(y) / smooth);
				for (int x = 0; x < smooth - y; x++) // v0-v2-v1
					vertices.push_back(VertexLerp(v02, v12, static_cast<float>(x) / (smooth - y)));
				vertices.push_back(v12); // v1-v2
				for (int x = 1; x <= y; x++) // v1-v2-v3
					vertices.push_back(VertexLerp(v12, v13, static_cast<float>(x) / y));
			}
			// add triangles
			for (int y = 0; y < smooth; y++)
			{
				for (int x = 0; x < smooth; x++)
				{
					int i0 = static_cast<int>(base_vi) + y * (smooth + 1) + x;
					int i1 = i0 + 1;
					int i2 = i0 + smooth + 1;
					int i3 = i2 + 1;
					triangles.push_back(i0); triangles.push_back(i2); triangles.push_back(i1);
					triangles.push_back(i2); triangles.push_back(i3); triangles.push_back(i1);
				}
			}
		};

		// front
		vertices.push_back(Vertex(Point(-1, -1, -1), Vector(0, 0, -1), GetColor(0), UV(0, 1), UV(0.25f, 0.5f)));
		vertices.push_back(Vertex(Point(1, -1, -1), Vector(0, 0, -1), GetColor(1), UV(1, 1), UV(0.5f, 0.5f)));
		vertices.push_back(Vertex(Point(-1, 1, -1), Vector(0, 0, -1), GetColor(2), UV(0, 0), UV(0.25f, 0.25f)));
		vertices.push_back(Vertex(Point(1, 1, -1), Vector(0, 0, -1), GetColor(3), UV(1, 0), UV(0.5f, 0.25f)));
		SmoothFace();
		// down
		vertices.push_back(Vertex(Point(-1, -1, 1), Vector(0, -1, 0), GetColor(4), UV(0, 1), UV(0.25f, 0.75f)));
		vertices.push_back(Vertex(Point(1, -1, 1), Vector(0, -1, 0), GetColor(5), UV(1, 1), UV(0.5f, 0.75f)));
		vertices.push_back(Vertex(Point(-1, -1, -1), Vector(0, -1, 0), GetColor(0), UV(0, 0), UV(0.25f, 0.5f)));
		vertices.push_back(Vertex(Point(1, -1, -1), Vector(0, -1, 0), GetColor(1), UV(1, 0), UV(0.5f, 0.5f)));
		SmoothFace();
		// up
		vertices.push_back(Vertex(Point(-1, 1, -1), Vector(0, 1, 0), GetColor(2), UV(0, 1), UV(0.25f, 0.25f)));
		vertices.push_back(Vertex(Point(1, 1, -1), Vector(0, 1, 0), GetColor(3), UV(1, 1), UV(0.5f, 0.25f)));
		vertices.push_back(Vertex(Point(-1, 1, 1), Vector(0, 1, 0), GetColor(6), UV(0, 0), UV(0.25f, 0)));
		vertices.push_back(Vertex(Point(1, 1, 1), Vector(0, 1, 0), GetColor(7), UV(1, 0), UV(0.5f, 0)));
		SmoothFace();
		// back
		vertices.push_back(Vertex(Point(-1, 1, 1), Vector(0, 0, 1), GetColor(6), UV(0, 1), UV(0.25f, 1)));
		vertices.push_back(Vertex(Point(1, 1, 1), Vector(0, 0, 1), GetColor(7), UV(1, 1), UV(0.5f, 1)));
		vertices.push_back(Vertex(Point(-1, -1, 1), Vector(0, 0, 1), GetColor(4), UV(0, 0), UV(0.25f, 0.75f)));
		vertices.push_back(Vertex(Point(1, -1, 1), Vector(0, 0, 1), GetColor(5), UV(1, 0), UV(0.5f, 0.75f)));
		SmoothFace();
		// left
		vertices.push_back(Vertex(Point(-1, -1, 1), Vector(-1, 0, 0), GetColor(4), UV(0, 1), UV(0, 0.5f)));
		vertices.push_back(Vertex(Point(-1, -1, -1), Vector(-1, 0, 0), GetColor(0), UV(1, 1), UV(0.25f, 0.5f)));
		vertices.push_back(Vertex(Point(-1, 1, 1), Vector(-1, 0, 0), GetColor(6), UV(0, 0), UV(0, 0.25f)));
		vertices.push_back(Vertex(Point(-1, 1, -1), Vector(-1, 0, 0), GetColor(2), UV(1, 0), UV(0.25f, 0.25f)));
		SmoothFace();
		// right
		vertices.push_back(Vertex(Point(1, -1, -1), Vector(1, 0, 0), GetColor(1), UV(0, 1), UV(0.5f, 0.5f)));
		vertices.push_back(Vertex(Point(1, -1, 1), Vector(1, 0, 0), GetColor(5), UV(1, 1), UV(0.75f, 0.5f)));
		vertices.push_back(Vertex(Point(1, 1, -1), Vector(1, 0, 0), GetColor(3), UV(0, 0), UV(0.5f, 0.25f)));
		vertices.push_back(Vertex(Point(1, 1, 1), Vector(1, 0, 0), GetColor(7), UV(1, 0), UV(0.75f, 0.25f)));
		SmoothFace();

		return std::make_shared<Mesh>(std::move(vertices), std::move(triangles));
	}

	std::shared_ptr<Mesh> CreateSphereMesh(int smooth)
	{
		smooth = Clamp(smooth, 2, 200);
		int yn = smooth, xn = 2 * smooth;

		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		// add vertices by latitude from top(0,1,0) to bottom(0,-1,0)
		for (int y = 0; y <= yn; y++)
		{
			float v = static_cast<float>(y) / yn;
			float theta = pi * v;
			for (int x = 0; x <= xn; x++)
			{
				float u = static_cast<float>(x) / xn;
				float phi = pi_mul2 * u;
				Point p(sinf(theta) * cosf(phi), cosf(theta), -sinf(theta) * sinf(phi));
				Vector n(p.x, p.y, p.z);
				vertices.emplace_back(p, n, Color::white, UV(u, v));
			}
		}

		// add first part triangles, which include top vertex
		for (int x = 0; x < xn; x++)
		{
			int a = x;
			int c = a + (xn + 1);
			int d = c + 1;
			triangles.push_back(a); triangles.push_back(c); triangles.push_back(d);
		}
		// add second part triangles
		for (int y = 1; y < yn - 1; y++)
		{
			for (int x = 0; x < xn; x++)
			{
				int a = y * (xn + 1) + x; // left upper vertex
				int b = a + 1;            // right upper vertex
				int c = a + (xn + 1);     // left below vertex
				int d = c + 1;            // right below vertex
				triangles.push_back(a); triangles.push_back(c); triangles.push_back(d);
				triangles.push_back(a); triangles.push_back(d); triangles.push_back(b);
			}
		}
		// add last part triangles, which include bottom vertex
		for (int x = 0; x < xn; x++)
		{
			int a = (yn - 1) * (xn + 1) + x;
			int b = a + 1;
			int c = a + (xn + 1);
			int d = c + 1;
			triangles.push_back(a); triangles.push_back(d); triangles.push_back(b);
		}

		return std::make_shared<Mesh>(std::move(vertices), std::move(triangles));
	}
	// generate internal triangles as return, and internal vertices will be added automatically
	std::vector<int> BigTriangleLerp(std::vector<Vertex>& vertices, const std::vector<int>& big_triangle, bool unit_vertex)
	{
		std::vector<int> triangles;
		size_t smooth = big_triangle.size() / 3;

		// add first part triangles
		triangles.push_back(big_triangle[0]); triangles.push_back(big_triangle[1]); triangles.push_back(big_triangle[3 * smooth - 1]);
		// add second part triangles
		for (size_t j = 1; j < smooth - 1; j++)
		{
			int a = big_triangle[j];                  // left upper vertex
			int b = big_triangle[j + 1];              // left below vertex
			int c = big_triangle[3 * smooth - j];     // right upper vertex
			int d = big_triangle[3 * smooth - j - 1]; // right below vertex
			Vertex vb = vertices[b], vd = vertices[d];
			for (size_t i = 0; i < j; i++)
			{
				// add vertex
				Vertex ve_new = VertexLerp(vb, vd, static_cast<float>(i + 1) / (j + 1));
				if (unit_vertex)
				{
					ve_new.p.w = 0;
					float length = VectorLength(ve_new.p);
					if (length != 0)
						ve_new.p /= length;
					ve_new.p.w = 1;
				}
				vertices.push_back(ve_new);
				int e = static_cast<int>(vertices.size()) - 1; // save new vertex in center below
				int f = e - static_cast<int>(j) + 1;         // load old vertex in center upper
				// add triangle
				triangles.push_back(a); triangles.push_back(b); triangles.push_back(e);
				if (i != j - 1)
				{
					triangles.push_back(a); triangles.push_back(e); triangles.push_back(f);
					a = f, b = e;
				}
				else
					b = e;
			}
			triangles.push_back(a); triangles.push_back(b); triangles.push_back(c);
			triangles.push_back(c); triangles.push_back(b); triangles.push_back(d);
		}
		// add last part triangles
		if (smooth > 1)
		{
			int a = big_triangle[smooth - 1];     // left upper vertex
			int b = big_triangle[smooth];         // left below vertex
			int c = big_triangle[2 * smooth + 1]; // right upper vertex
			int d = big_triangle[2 * smooth];     // right below vertex
			int f = static_cast<int>(vertices.size() - smooth) + 2; // load old vertex in center upper
			for (size_t i = 0; i < smooth - 1; i++)
			{
				int e = big_triangle[smooth + i + 1]; // center below vertex
				triangles.push_back(a); triangles.push_back(b); triangles.push_back(e);
				if (i != smooth - 2)
				{
					triangles.push_back(a); triangles.push_back(e); triangles.push_back(f);
					a = f++, b = e;
				}
				else
					b = e;
			}
			triangles.push_back(a); triangles.push_back(b); triangles.push_back(c);
			triangles.push_back(c); triangles.push_back(b); triangles.push_back(d);
		}

		return triangles;
	}
	// compute sphere skeleton for two key vertex
	void SphereSkeleton(std::vector<Vertex>& vertices, int smooth, int a, int b)
	{
		for (int i = 1; i < smooth; i++)
		{
			Vertex v_new = VertexLerp(vertices[a], vertices[b], static_cast<float>(i) / smooth);
			v_new.p.w = 0;
			float length = VectorLength(v_new.p);
			if (length != 0)
				v_new.p /= length;
			v_new.p.w = 1;
			vertices.push_back(v_new);
		}
	}
	// generate vertex indices of big triangle using three key vertex and three edge skeleton
	// vn = key vertex number, it's skeleton's index offset
	// a,b,c = key vertex index
	// eab,ebc,eca = skeleton index, op means order
	std::vector<int> SphereBigTriangle(int smooth, int vn, int a, int b, int c, int eab, int ebc, int eca)
	{
		std::vector<int> big_tri;
		auto PushBackEdge = [&big_tri, smooth, vn](int e)
		{
			int o = vn + (abs(e) - 1) * (smooth - 1);
			if (e > 0)
			{
				for (int i = 0; i < smooth - 1; i++)
					big_tri.push_back(i + o);
			}
			else
			{
				for (int i = smooth - 2; i >= 0; i--)
					big_tri.push_back(i + o);
			}
		};
		big_tri.push_back(a);
		PushBackEdge(eab);
		big_tri.push_back(b);
		PushBackEdge(ebc);
		big_tri.push_back(c);
		PushBackEdge(eca);
		return big_tri;
	}
	void TrianglesAppend(std::vector<int>& triangles, const std::vector<int>& _triangles)
	{
		triangles.insert(triangles.end(), _triangles.begin(), _triangles.end());
	}
	std::shared_ptr<Mesh> CreateSphereMeshB(int smooth)
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		// add first part vertices
		vertices.emplace_back(Point(0, 1, 0), Color::white, UV(4 / 7.0f, 1 / 3.0f));  // 0
		vertices.emplace_back(Point(1, 0, 0), Color::white, UV(3 / 7.0f, 2 / 3.0f));  // 1
		vertices.emplace_back(Point(0, 0, -1), Color::white, UV(5 / 7.0f, 2 / 3.0f)); // 2
		vertices.emplace_back(Point(-1, 0, 0), Color::white, UV(6 / 7.0f, 1 / 3.0f)); // 3
		vertices.emplace_back(Point(0, 0, 1), Color::white, UV(2 / 7.0f, 1 / 3.0f));  // 4
		vertices.emplace_back(Point(0, -1, 0), Color::white, UV(1 / 7.0f, 2 / 3.0f)); // 5
		vertices.emplace_back(Point(-1, 0, 0), Color::white, UV(3 / 7.0f, 0 / 3.0f)); // 6 = 3
		vertices.emplace_back(Point(-1, 0, 0), Color::white, UV(0 / 7.0f, 1 / 3.0f)); // 7 = 3
		vertices.emplace_back(Point(0, -1, 0), Color::white, UV(4 / 7.0f, 3 / 3.0f)); // 8 = 5
		vertices.emplace_back(Point(0, -1, 0), Color::white, UV(7 / 7.0f, 2 / 3.0f)); // 9 = 5
		// add second part vertices
		SphereSkeleton(vertices, smooth, 0, 1);                                         // 1
		SphereSkeleton(vertices, smooth, 0, 2); SphereSkeleton(vertices, smooth, 1, 2); // 2  3
		SphereSkeleton(vertices, smooth, 0, 4); SphereSkeleton(vertices, smooth, 1, 4); // 4  5
		SphereSkeleton(vertices, smooth, 0, 3); SphereSkeleton(vertices, smooth, 2, 3); // 6  7
		SphereSkeleton(vertices, smooth, 1, 5); SphereSkeleton(vertices, smooth, 4, 5); // 8  9
		SphereSkeleton(vertices, smooth, 0, 6); SphereSkeleton(vertices, smooth, 4, 6); // 10 11
		SphereSkeleton(vertices, smooth, 4, 7); SphereSkeleton(vertices, smooth, 5, 7); // 12 13
		SphereSkeleton(vertices, smooth, 1, 8); SphereSkeleton(vertices, smooth, 2, 8); // 14 15
		SphereSkeleton(vertices, smooth, 2, 9); SphereSkeleton(vertices, smooth, 3, 9); // 16 17
		// update normal
		for (auto& v : vertices)
		{
			v.n = v.p;
			v.n.w = 0;
		}
		// add triangles
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 10, 0, 1, 2, +1, +3, -2), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 10, 1, 0, 4, -1, +4, -5), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 10, 0, 2, 3, +2, +7, -6), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 10, 1, 4, 5, +5, +9, -8), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 10, 4, 0, 6, -4, +10, -11), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 10, 5, 4, 7, -9, +12, -13), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 10, 2, 1, 8, -3, +14, -15), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 10, 3, 2, 9, -7, +16, -17), true));

		return std::make_shared<Mesh>(std::move(vertices), std::move(triangles));
	}
	std::shared_ptr<Mesh> CreateSphereMeshC(int smooth)
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		// add first part vertices
		float sqr2 = sqrtf(2), sqr6 = sqrtf(6);
		vertices.emplace_back(Point(0, 1, 0), Color::white, UV(2 / 4.0f, 0 / 2.0f));                         // 0
		vertices.emplace_back(Point(2 * sqr2 / 3, -1.0f / 3, 0), Color::white, UV(1 / 4.0f, 1 / 2.0f));      // 1
		vertices.emplace_back(Point(-sqr2 / 3, -1.0f / 3, -sqr6 / 3), Color::white, UV(3 / 4.0f, 1 / 2.0f)); // 2
		vertices.emplace_back(Point(-sqr2 / 3, -1.0f / 3, +sqr6 / 3), Color::white, UV(0 / 4.0f, 0 / 2.0f)); // 3
		vertices.emplace_back(Point(-sqr2 / 3, -1.0f / 3, +sqr6 / 3), Color::white, UV(4 / 4.0f, 0 / 2.0f)); // 4 = 3
		vertices.emplace_back(Point(-sqr2 / 3, -1.0f / 3, +sqr6 / 3), Color::white, UV(2 / 4.0f, 2 / 2.0f)); // 5 = 3
		// add second part vertices
		SphereSkeleton(vertices, smooth, 0, 1);                                         // 1
		SphereSkeleton(vertices, smooth, 0, 2); SphereSkeleton(vertices, smooth, 1, 2); // 2  3
		SphereSkeleton(vertices, smooth, 0, 3); SphereSkeleton(vertices, smooth, 1, 3); // 4  5
		SphereSkeleton(vertices, smooth, 0, 4); SphereSkeleton(vertices, smooth, 2, 4); // 6  7
		SphereSkeleton(vertices, smooth, 1, 5); SphereSkeleton(vertices, smooth, 2, 5); // 8  9
		// update normal
		for (auto& v : vertices)
		{
			v.n = v.p;
			v.n.w = 0;
		}
		// add triangles
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 0, 1, 2, +1, +3, -2), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 1, 0, 3, -1, +4, -5), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 0, 2, 4, +2, +7, -6), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 2, 1, 5, -3, +8, -9), true));

		return std::make_shared<Mesh>(std::move(vertices), std::move(triangles));
	}
	std::shared_ptr<Mesh> CreateSphereMeshD(int smooth)
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		// add first part vertices
		float tau = (1 + sqrtf(5)) / 2;
		vertices.emplace_back(Point(0, +tau, +1), Color::white, UV(1 / 11.0f, 0 / 3.0f));  // 0 (- 1,2,3,4,5)
		vertices.emplace_back(Point(0, +tau, -1), Color::white, UV(2 / 11.0f, 1 / 3.0f));  // 1 (- 0,5,6,7,2)
		vertices.emplace_back(Point(-tau, +1, 0), Color::white, UV(4 / 11.0f, 1 / 3.0f));  // 2 (- 0,1,7,8,3)
		vertices.emplace_back(Point(-1, 0, +tau), Color::white, UV(6 / 11.0f, 1 / 3.0f));  // 3
		vertices.emplace_back(Point(+1, 0, +tau), Color::white, UV(8 / 11.0f, 1 / 3.0f));  // 4
		vertices.emplace_back(Point(+tau, +1, 0), Color::white, UV(0 / 11.0f, 1 / 3.0f));  // 5
		vertices.emplace_back(Point(+1, 0, -tau), Color::white, UV(1 / 11.0f, 2 / 3.0f));  // 6
		vertices.emplace_back(Point(-1, 0, -tau), Color::white, UV(3 / 11.0f, 2 / 3.0f));  // 7
		vertices.emplace_back(Point(-tau, -1, 0), Color::white, UV(5 / 11.0f, 2 / 3.0f));  // 8
		vertices.emplace_back(Point(0, -tau, +1), Color::white, UV(7 / 11.0f, 2 / 3.0f));  // 9
		vertices.emplace_back(Point(+tau, -1, 0), Color::white, UV(9 / 11.0f, 2 / 3.0f));  // 10
		vertices.emplace_back(Point(0, -tau, -1), Color::white, UV(2 / 11.0f, 3 / 3.0f));  // 11 (- 6,7,8,9,10)
		vertices.emplace_back(Point(0, +tau, +1), Color::white, UV(3 / 11.0f, 0 / 3.0f));  // 12 = 0
		vertices.emplace_back(Point(0, +tau, +1), Color::white, UV(5 / 11.0f, 0 / 3.0f));  // 13 = 0
		vertices.emplace_back(Point(0, +tau, +1), Color::white, UV(7 / 11.0f, 0 / 3.0f));  // 14 = 0
		vertices.emplace_back(Point(0, +tau, +1), Color::white, UV(9 / 11.0f, 0 / 3.0f));  // 15 = 0
		vertices.emplace_back(Point(+tau, +1, 0), Color::white, UV(10 / 11.0f, 1 / 3.0f)); // 16 = 5
		vertices.emplace_back(Point(+1, 0, -tau), Color::white, UV(11 / 11.0f, 2 / 3.0f)); // 17 = 6
		vertices.emplace_back(Point(0, -tau, -1), Color::white, UV(4 / 11.0f, 3 / 3.0f));  // 18 = 11
		vertices.emplace_back(Point(0, -tau, -1), Color::white, UV(6 / 11.0f, 3 / 3.0f));  // 19 = 11
		vertices.emplace_back(Point(0, -tau, -1), Color::white, UV(8 / 11.0f, 3 / 3.0f));  // 20 = 11
		vertices.emplace_back(Point(0, -tau, -1), Color::white, UV(10 / 11.0f, 3 / 3.0f)); // 21 = 11
		for (auto& v : vertices)
		{
			v.p.w = 0;
			float length = VectorLength(v.p);
			if (length != 0)
				v.p /= length;
			v.p.w = 1;
		}
		// add second part vertices
		SphereSkeleton(vertices, smooth, 0, 5); SphereSkeleton(vertices, smooth, 0, 1);    // 1  2
		SphereSkeleton(vertices, smooth, 12, 1); SphereSkeleton(vertices, smooth, 12, 2);  // 3  4
		SphereSkeleton(vertices, smooth, 13, 2); SphereSkeleton(vertices, smooth, 13, 3);  // 5  6
		SphereSkeleton(vertices, smooth, 14, 3); SphereSkeleton(vertices, smooth, 14, 4);  // 7  8
		SphereSkeleton(vertices, smooth, 15, 4); SphereSkeleton(vertices, smooth, 15, 16); // 9  10
		SphereSkeleton(vertices, smooth, 5, 1); SphereSkeleton(vertices, smooth, 1, 2);    // 11 12
		SphereSkeleton(vertices, smooth, 2, 3); SphereSkeleton(vertices, smooth, 3, 4);    // 13 14
		SphereSkeleton(vertices, smooth, 4, 16);                                           // 15
		SphereSkeleton(vertices, smooth, 5, 6); SphereSkeleton(vertices, smooth, 6, 1);    // 16 17
		SphereSkeleton(vertices, smooth, 1, 7); SphereSkeleton(vertices, smooth, 7, 2);    // 18 19
		SphereSkeleton(vertices, smooth, 2, 8); SphereSkeleton(vertices, smooth, 8, 3);    // 20 21
		SphereSkeleton(vertices, smooth, 3, 9); SphereSkeleton(vertices, smooth, 9, 4);    // 22 23
		SphereSkeleton(vertices, smooth, 4, 10); SphereSkeleton(vertices, smooth, 10, 16); // 24 25
		SphereSkeleton(vertices, smooth, 16, 17);                                          // 26
		SphereSkeleton(vertices, smooth, 6, 7); SphereSkeleton(vertices, smooth, 7, 8);    // 27 28
		SphereSkeleton(vertices, smooth, 8, 9); SphereSkeleton(vertices, smooth, 9, 10);   // 29 30
		SphereSkeleton(vertices, smooth, 10, 17);                                          // 31
		SphereSkeleton(vertices, smooth, 6, 11); SphereSkeleton(vertices, smooth, 7, 11);  // 32 33
		SphereSkeleton(vertices, smooth, 7, 18); SphereSkeleton(vertices, smooth, 8, 18);  // 34 35
		SphereSkeleton(vertices, smooth, 8, 19); SphereSkeleton(vertices, smooth, 9, 19);  // 36 37
		SphereSkeleton(vertices, smooth, 9, 20); SphereSkeleton(vertices, smooth, 10, 20); // 38 39
		SphereSkeleton(vertices, smooth, 10, 21); SphereSkeleton(vertices, smooth, 17, 21);// 40 41
		// update normal
		for (auto& v : vertices)
		{
			v.n = v.p;
			v.n.w = 0;
		}
		// add triangles
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 0, 5, 1, +1, +11, -2), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 12, 1, 2, +3, +12, -4), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 13, 2, 3, +5, +13, -6), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 14, 3, 4, +7, +14, -8), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 15, 4, 16, +9, +15, -10), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 1, 5, 6, -11, +16, +17), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 1, 6, 7, -17, +27, -18), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 2, 1, 7, -12, +18, +19), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 2, 7, 8, -19, +28, -20), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 3, 2, 8, -13, +20, +21), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 3, 8, 9, -21, +29, -22), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 4, 3, 9, -14, +22, +23), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 4, 9, 10, -23, +30, -24), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 16, 4, 10, -15, +24, +25), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 16, 10, 17, -25, +31, -26), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 7, 6, 11, -27, +32, -33), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 8, 7, 18, -28, +34, -35), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 9, 8, 19, -29, +36, -37), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 10, 9, 20, -30, +38, -39), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 22, 17, 10, 21, -31, +40, -41), true));

		return std::make_shared<Mesh>(std::move(vertices), std::move(triangles));
	}
	std::shared_ptr<Mesh> CreateMeshFromObjFile(const std::string& filename)
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		std::ifstream fs(filename.c_str());
		if (fs.fail())
			return nullptr;

		std::vector<Point> vs;
		std::vector<UV> vts;
		std::vector<Vector> vns;

		while (fs.good())
		{
			std::string line;
			std::getline(fs, line);
			std::istringstream ss(line);

			std::string str;
			ss >> str;
			if (str.length() == 0) // empty line
				continue;
			else if (str[0] == '#') // comment line
				continue;
			else if (str == "mtlib") // material // ignore
				continue;
			else if (str == "newmtl") // material // ignore
				continue;
			else if (str == "usemtl") // material // ignore
				continue;
			else if (str == "g") // group // ignore
				continue;
			else if (str == "v") // pos
			{
				Point p;
				ss >> p.x >> p.y >> p.z;
				vs.push_back(p);
			}
			else if (str == "vt") // uv
			{
				UV uv;
				ss >> uv.x >> uv.y;
				vts.push_back(uv);
			}
			else if (str == "vn") // normal
			{
				Vector n;
				ss >> n.x >> n.y >> n.z;
				vns.push_back(n);
			}
			else if (str == "f") // face
			{
				for (int i = 0; i < 4; i++)
				{
					int values[3]{ 0,0,0 };

					// read
					std::string part;
					if (!(ss >> part))
						break;
					std::istringstream part_ss(part);
					for (int j = 0; j < 3; j++)
					{
						std::string value_str;
						getline(part_ss, value_str, '/');
						if (value_str.length() != 0)
							values[j] = std::stoi(value_str);
					}
					for (int j = 0; j < 3; j++)
					{
						if (values[j] >= 0)
							values[j] -= 1;
						else
						{
							if (j == 0)
								values[j] += static_cast<int>(vs.size());
							else if (j == 1)
								values[j] += static_cast<int>(vts.size());
							else if (j == 2)
								values[j] += static_cast<int>(vns.size());
						}
					}

					// add vertex
					if (values[0] < 0 || values[0] >= vs.size()) // no pos, ignore the face
						break;
					Vertex v(vs[values[0]]);
					if (values[2] >= 0 && values[2] < vns.size())
						v.n = vns[values[2]];
					if (values[1] >= 0 && values[1] < vts.size())
						v.uv = vts[values[1]];
					v.c = Color::white;
					vertices.push_back(v);

					// add triangle
					if (i == 2)
					{
						int vi = static_cast<int>(vertices.size()) - 1;
						triangles.push_back(vi - 2); triangles.push_back(vi - 1); triangles.push_back(vi);
					}
					else if (i == 3)
					{
						int vi = static_cast<int>(vertices.size()) - 1;
						triangles.push_back(vi - 3); triangles.push_back(vi - 1); triangles.push_back(vi);
					}
				}
			}
			else if (str == "Ka") // material // ignore
				continue;
			else if (str == "Kd") // material // ignore
				continue;
			else if (str == "d") // material // ignore
				continue;
			else if (str == "illum") // illumination // ignore
				continue;
			else if (str.substr(0, 4) == "map_") // texture // ignore
				continue;
			else // other // ignore
				continue;
		}

		return std::make_shared<Mesh>(std::move(vertices), std::move(triangles));
	}

	std::shared_ptr<Mesh> CreateFrustumMesh(float top_radius, int smooth)
	{
		top_radius = Clamp(top_radius, 0.0f, 1.0f);
		smooth = Clamp(smooth, 2, 200);
		int yn = smooth, xn = 2 * smooth;
		int brn = yn, trn = Max(1, static_cast<int>(yn * top_radius));

		std::vector<Vertex> vertices;
		std::vector<int> triangles;
		int base_vi;

		// add top disc
		if (top_radius > 0)
		{
			for (int r = 0; r <= trn; r++)
			{
				float t = static_cast<float>(r) / trn;
				float v = t * 0.25f;
				float radius = t * top_radius;
				for (int x = 0; x <= xn; x++)
				{
					float u = static_cast<float>(x) / xn;
					float phi = pi_mul2 * u;
					Point p(radius * cosf(phi), 1, -radius * sinf(phi));
					vertices.emplace_back(p, Vector(0, 1, 0), Color::white, UV(u, v));
				}
			}
			for (int r = 0; r < trn; r++)
			{
				for (int x = 0; x < xn; x++)
				{
					int a = r * (xn + 1) + x;
					int b = a + 1;
					int c = a + (xn + 1);
					int d = c + 1;
					triangles.push_back(a); triangles.push_back(c); triangles.push_back(d);
					if (r != 0)
					{
						triangles.push_back(a); triangles.push_back(d); triangles.push_back(b);
					}
				}
			}
		}
		// add side face
		Vector normal(2, 1 - top_radius, 0);
		normal = VectorNormalize(normal);
		base_vi = static_cast<int>(vertices.size());
		for (int y = 0; y <= yn; y++)
		{
			float t = static_cast<float>(y) / yn;
			float v = t * 0.5f + 0.25f;
			float height = 1 - 2 * t;
			float radius = Lerp(top_radius, 1.0f, t);
			for (int x = 0; x <= xn; x++)
			{
				float u = static_cast<float>(x) / xn;
				float phi = pi_mul2 * u;
				Point p(radius * cosf(phi), height, -radius * sinf(phi));
				Vector n(normal.x * cosf(phi), normal.y, -normal.x * sinf(phi));
				vertices.emplace_back(p, n, Color::white, UV(u, v));
			}
		}
		for (int y = 0; y < yn; y++)
		{
			for (int x = 0; x < xn; x++)
			{
				int a = base_vi + y * (xn + 1) + x;
				int b = a + 1;
				int c = a + (xn + 1);
				int d = c + 1;
				triangles.push_back(a); triangles.push_back(c); triangles.push_back(d);
				if (top_radius != 0 || y != 0)
				{
					triangles.push_back(a); triangles.push_back(d); triangles.push_back(b);
				}
			}
		}
		// add bottom disc
		base_vi = static_cast<int>(vertices.size());
		if (top_radius > 0)
		{
			for (int r = 0; r <= brn; r++)
			{
				float t = static_cast<float>(r) / trn;
				float v = t * 0.25f + 0.75f;
				float radius = 1 - t;
				for (int x = 0; x <= xn; x++)
				{
					float u = static_cast<float>(x) / xn;
					float phi = pi_mul2 * u;
					Point p(radius * cosf(phi), -1, -radius * sinf(phi));
					vertices.emplace_back(p, Vector(0, -1, 0), Color::white, UV(u, v));
				}
			}
			for (int r = 0; r < brn; r++)
			{
				for (int x = 0; x < xn; x++)
				{
					int a = base_vi + r * (xn + 1) + x;
					int b = a + 1;
					int c = a + (xn + 1);
					int d = c + 1;
					if (r != brn - 1)
					{
						triangles.push_back(a); triangles.push_back(c); triangles.push_back(d);
					}
					triangles.push_back(a); triangles.push_back(d); triangles.push_back(b);
				}
			}
		}

		return std::make_shared<Mesh>(std::move(vertices), std::move(triangles));
	}

	Texture::Texture(int _width, int _height) : width(_width), height(_height), buffer(new Color[static_cast<size_t>(_width) * _height])
	{
	}
	Texture::Texture(const Texture& t) : width(t.width), height(t.height), buffer(new Color[static_cast<size_t>(t.width) * t.height])
	{
		std::copy(t.buffer, t.buffer + static_cast<size_t>(width) * height, buffer);
	}
	Texture::~Texture()
	{
		delete[] buffer;
	}
	Color Texture::GetColor(float u, float v)
	{
		int x = static_cast<int>(u * width);
		int y = static_cast<int>(v * height);
		x = Clamp(x, 0, width - 1);
		y = Clamp(y, 0, height - 1);
		return buffer[y * width + x];
	}
	Color Texture::GetColor(UV uv)
	{
		return GetColor(uv.x, uv.y);
	}
	std::shared_ptr<Texture> CreateTexture1()
	{
		char image1[49]{
			0,0,0,0,0,0,0,
			0,0,1,0,0,0,0,
			0,1,1,0,0,0,0,
			0,0,1,0,0,0,0,
			0,0,1,0,0,0,0,
			0,1,1,1,0,0,0,
			0,0,0,0,0,0,0
		};
		auto pt = std::make_shared<Texture>(7, 7);
		for (int i = 0; i < 49; i++)
			pt->buffer[i] = (image1[i] + 1) / 2.0f * Color(1, 1, 1);
		return pt;
	}
	std::shared_ptr<Texture> CreateTextureC()
	{
		char imageC[49]{
			0,0,0,0,0,0,0,
			0,0,1,1,0,0,0,
			0,1,0,0,1,0,0,
			0,1,0,0,0,0,0,
			0,1,0,0,1,0,0,
			0,0,1,1,0,0,0,
			0,0,0,0,0,0,0
		};
		auto pt = std::make_shared<Texture>(7, 7);
		for (int i = 0; i < 49; i++)
			pt->buffer[i] = (imageC[i] + 1) / 2.0f * Color(1, 1, 1);
		return pt;
	}
	std::shared_ptr<Texture> CreateTexturePlaid(int n)
	{
		auto pt = std::make_shared<Texture>(n, n);
		for (int y = 0; y < n; y++)
		{
			for (int x = 0; x < n; x++)
			{
				int i = y * n + x;
				if ((x + y) % 2 == 0)
					pt->buffer[i] = Color(1, 1, 1);
				else
					pt->buffer[i] = Color(0, 0, 1);
			}
		}
		return pt;
	}
	std::shared_ptr<Texture> CreateTextureDice()
	{
		const int size = 28;
		char image[size * size]{
			0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,0,0,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,0,0,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,

			0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,1,0,1,0,0, 0,0,0,0,1,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0,
			0,0,1,0,1,0,0, 0,0,0,0,1,0,0, 0,0,1,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,1,1,1,0,0, 0,0,0,0,1,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,1,0,0, 0,0,0,0,1,0,0, 0,0,0,0,1,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,1,0,0, 0,0,0,0,1,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,

			0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,0,0,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,

			0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,0,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,1,1,1,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0,
			0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0, 0,0,0,0,0,0,0
		};
		auto pt = std::make_shared<Texture>(size, size);
		for (int i = 0; i < size * size; i++)
			pt->buffer[i] = (image[i] + 1) / 2.0f * Color(1, 1, 1);
		return pt;
	}
}