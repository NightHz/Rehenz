#include "render_soft.h"

namespace Rehenz
{
	// save all objects to render
	std::vector<std::shared_ptr<Object>> objs;


	Vertex::Vertex(Point _p) : p(_p)
	{
	}

	Mesh::Mesh() : vertices(), triangles()
	{
	}
	Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<int>& _triangles) : vertices(_vertices), triangles(_triangles)
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
	Object::Object() : pmesh(nullptr), position(), rotation(), scale(1, 1, 1, 0)
	{
	}
	Object::Object(std::shared_ptr<Mesh> _pmesh) : pmesh(_pmesh), position(), rotation(), scale(1, 1, 1, 0)
	{
	}
	Object::~Object()
	{
	}
	Camera::Camera() : position(0, 0, -5, 0), at(0, 0, 1, 0), up(0, 1, 0, 0)
	{
		height = 600;
		width = 800;
		int size = height * width;
		buffer = new uint[size];
		fovy = pi / 2;
		aspect = 4.0f / 3;
		z_near = 1;
		z_far = 500;
	}
	Camera::Camera(int _height, int _width) : position(0, 0, -5, 0), at(0, 0, 1, 0), up(0, 1, 0, 0)
	{
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];
		fovy = pi / 2;
		aspect = static_cast<float>(width) / height;
		z_near = 1;
		z_far = 500;
	}
	Camera::~Camera()
	{
		delete[] buffer;
	}
	void Camera::SetSize(int _height, int _width, float _aspect)
	{
		delete[] buffer;
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];
		aspect = _aspect;
	}
	const uint* Camera::RenderImage()
	{
		// Copy and transform all objects (vertex shader)
		std::vector<Vertex> vertices;
		std::vector<int> triangles;
		Matrix mat_view = GetInverseMatrixT(position) * GetInverseMatrixR(at, up) * GetMatrixP(fovy, aspect, z_near, z_far);
		for (auto pobj : objs)
		{
			Matrix mat_world = GetMatrixS(pobj->scale) * GetMatrixE(pobj->rotation) * GetMatrixT(pobj->position);
			Matrix transform = mat_world * mat_view;
			auto& vs_mesh = pobj->pmesh->GetVertices();
			auto& tris_mesh = pobj->pmesh->GetTriangles();
			int v_index_offset = static_cast<int>(vertices.size());
			for (auto& v : vs_mesh)
				vertices.push_back(Vertex(PointStandard(v.p * transform)));
			for (auto tri : tris_mesh)
				triangles.push_back(tri + v_index_offset);
		}

		// Clipping and back-face culling
		std::vector<int> tris_new;
		for (size_t i = 0; i < triangles.size(); i += 3)
		{
			int a = triangles[i], b = triangles[i + 1], c = triangles[i + 2];
			Vertex& va = vertices[a], & vb = vertices[b], & vc = vertices[c];
			auto Inside = [](Vertex& v) -> bool { return v.p.x >= -1 && v.p.x <= 1 && v.p.y >= -1 && v.p.y <= 1 && v.p.z >= 0 && v.p.z <= 1; };
			if (Inside(va) && Inside(vb) && Inside(vc) && TrianglesNormal(va.p, vb.p, vc.p).z < 0)
			{
				tris_new.push_back(a); tris_new.push_back(b); tris_new.push_back(c);
			}
		}
		triangles.swap(tris_new);
		tris_new.clear();

		// Mapping to screen
		std::vector<Point2I> screen_pos;
		for (auto& v : vertices)
		{
			// (-1,-1) -> (0,h-1), (1,1) -> (w-1,0)
			screen_pos.push_back(Point2I(static_cast<int>((v.p.x + 1) * (width - 1) / 2), static_cast<int>((-v.p.y + 1) * (height - 1) / 2)));
		}

		// Traverse all triangles and compute color for all sampling points (pixel shader)
		int size = height * width;
		float* zbuffer = new float[size];
		std::fill(zbuffer, zbuffer + size, 2.0f);
		std::fill(buffer, buffer + size, 0U);
		for (size_t i = 0; i < triangles.size(); i += 3)
		{
			int a = triangles[i], b = triangles[i + 1], c = triangles[i + 2];
			Vertex& va = vertices[a], & vb = vertices[b], & vc = vertices[c];
			Point2I pa = screen_pos[a], pb = screen_pos[b], pc = screen_pos[c];
			// Use z-buffer merge multiple colors
			DrawerZ drawer(buffer, width, height, zbuffer);
			drawer.Line(pa, pb, Drawer::Color(255, 255, 255), va.p.z, vb.p.z);
			drawer.Line(pa, pc, Drawer::Color(255, 255, 255), va.p.z, vc.p.z);
			drawer.Line(pb, pc, Drawer::Color(255, 255, 255), vb.p.z, vc.p.z);
		}
		delete[] zbuffer;

		return buffer;
	}
	Vertex VertexLerp(const Vertex& v1, const Vertex& v2, float t)
	{
		return Vertex(PointLerp(v1.p, v2.p, t));
	}
	std::shared_ptr<Mesh> CreateCubeMesh()
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		vertices.push_back(Point(0, 0, 0, 1));
		vertices.push_back(Point(0, 0, 1, 1));
		vertices.push_back(Point(1, 0, 0, 1));
		vertices.push_back(Point(1, 0, 1, 1));
		vertices.push_back(Point(0, 1, 0, 1));
		vertices.push_back(Point(0, 1, 1, 1));
		vertices.push_back(Point(1, 1, 0, 1));
		vertices.push_back(Point(1, 1, 1, 1));

		triangles.push_back(0); triangles.push_back(2); triangles.push_back(1);
		triangles.push_back(2); triangles.push_back(3); triangles.push_back(1);
		triangles.push_back(0); triangles.push_back(1); triangles.push_back(4);
		triangles.push_back(1); triangles.push_back(5); triangles.push_back(4);
		triangles.push_back(0); triangles.push_back(4); triangles.push_back(2);
		triangles.push_back(4); triangles.push_back(6); triangles.push_back(2);
		triangles.push_back(7); triangles.push_back(6); triangles.push_back(5);
		triangles.push_back(6); triangles.push_back(4); triangles.push_back(5);
		triangles.push_back(7); triangles.push_back(3); triangles.push_back(6);
		triangles.push_back(3); triangles.push_back(2); triangles.push_back(6);
		triangles.push_back(7); triangles.push_back(5); triangles.push_back(3);
		triangles.push_back(5); triangles.push_back(1); triangles.push_back(3);

		return std::make_shared<Mesh>(vertices, triangles);
	}
	std::shared_ptr<Mesh> CreateSphereMesh(int smooth)
	{
		smooth = Clamp(smooth, 2, 200);
		int yn = smooth, xn = 2 * smooth;

		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		// add vertices by latitude from top(0,1,0) to bottom(0,-1,0)
		vertices.push_back(Point(0, 1, 0));
		for (int y = 1; y < yn; y++)
		{
			float theta = pi * y / yn;
			for (int x = 0; x < xn; x++)
			{
				float phi = 2 * pi * x / xn;
				vertices.push_back(Point(sinf(theta) * cosf(phi), cosf(theta), -sinf(theta) * sinf(phi)));
			}
		}
		vertices.push_back(Point(0, -1, 0));

		// add first part triangles, which include top vertex
		for (int i = 1; i < xn; i++)
		{
			triangles.push_back(0); triangles.push_back(i); triangles.push_back(i + 1);
		}
		triangles.push_back(0); triangles.push_back(xn); triangles.push_back(1);
		// add second part triangles
		for (int j = 2; j < yn; j++)
		{
			int a = 1 + xn * (j - 2); // upper starting vertex
			int b = 1 + xn * (j - 1); // below starting vertex
			for (int i = 0; i < xn - 1; i++)
			{
				triangles.push_back(a + i); triangles.push_back(b + i); triangles.push_back(b + i + 1);
				triangles.push_back(a + i); triangles.push_back(b + i + 1); triangles.push_back(a + i + 1);
			}
			triangles.push_back(a + xn - 1); triangles.push_back(b + xn - 1); triangles.push_back(b);
			triangles.push_back(a + xn - 1); triangles.push_back(b); triangles.push_back(a);
		}
		// add last part triangles, which include bottom vertex
		int bottom = xn * (yn - 1) + 1;
		int a = 1 + xn * (yn - 2); // starting vertex
		for (int i = 0; i < xn - 1; i++)
		{
			triangles.push_back(a + i + 1); triangles.push_back(a + i); triangles.push_back(bottom);
		}
		triangles.push_back(a); triangles.push_back(a + xn - 1); triangles.push_back(bottom);

		return std::make_shared<Mesh>(vertices, triangles);
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
			for (int i = 0; i < j; i++)
			{
				// add vertex
				Vertex ve_new = VertexLerp(vb, vd, static_cast<float>(i + 1) / (j + 1));
				if (unit_vertex)
				{
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
			for (int i = 0; i < smooth - 1; i++)
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
	void SphereSkeleton(std::vector<Vertex>& vertices, int smooth, int a, int b)
	{
		for (int i = 1; i < smooth; i++)
		{
			Vertex v_new = VertexLerp(vertices[a], vertices[b], static_cast<float>(i) / smooth);
			float length = VectorLength(v_new.p);
			if (length != 0)
				v_new.p /= length;
			v_new.p.w = 1;
			vertices.push_back(v_new);
		}
	}
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
		vertices.push_back(Point(0, 1, 0));
		vertices.push_back(Point(1, 0, 0));
		vertices.push_back(Point(0, 0, -1));
		vertices.push_back(Point(-1, 0, 0));
		vertices.push_back(Point(0, 0, 1));
		vertices.push_back(Point(0, -1, 0));
		// add second part vertices
		SphereSkeleton(vertices, smooth, 0, 1); SphereSkeleton(vertices, smooth, 0, 2);
		SphereSkeleton(vertices, smooth, 0, 3); SphereSkeleton(vertices, smooth, 0, 4);
		SphereSkeleton(vertices, smooth, 1, 2); SphereSkeleton(vertices, smooth, 2, 3);
		SphereSkeleton(vertices, smooth, 3, 4); SphereSkeleton(vertices, smooth, 4, 1);
		SphereSkeleton(vertices, smooth, 1, 5); SphereSkeleton(vertices, smooth, 2, 5);
		SphereSkeleton(vertices, smooth, 3, 5); SphereSkeleton(vertices, smooth, 4, 5);
		// add triangles
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 0, 1, 2, +1, +5, -2), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 0, 2, 3, +2, +6, -3), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 0, 3, 4, +3, +7, -4), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 0, 4, 1, +4, +8, -1), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 5, 2, 1, -10, -5, +9), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 5, 3, 2, -11, -6, +10), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 5, 4, 3, -12, -7, +11), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 6, 5, 1, 4, -9, -8, +12), true));

		return std::make_shared<Mesh>(vertices, triangles);
	}
	std::shared_ptr<Mesh> CreateSphereMeshC(int smooth)
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		// add first part vertices
		float sqr2 = sqrtf(2), sqr6 = sqrtf(6);
		vertices.push_back(Point(0, 1, 0));
		vertices.push_back(Point(2 * sqr2 / 3, -1.0f / 3, 0));
		vertices.push_back(Point(-sqr2 / 3, -1.0f / 3, -sqr6 / 3));
		vertices.push_back(Point(-sqr2 / 3, -1.0f / 3, +sqr6 / 3));
		// add second part vertices
		SphereSkeleton(vertices, smooth, 0, 1); SphereSkeleton(vertices, smooth, 0, 2);
		SphereSkeleton(vertices, smooth, 0, 3); SphereSkeleton(vertices, smooth, 1, 2);
		SphereSkeleton(vertices, smooth, 2, 3); SphereSkeleton(vertices, smooth, 3, 1);
		// add triangles
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 4, 0, 1, 2, +1, +4, -2), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 4, 0, 2, 3, +2, +5, -3), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 4, 0, 3, 1, +3, +6, -1), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 4, 1, 3, 2, -6, -5, -4), true));

		return std::make_shared<Mesh>(vertices, triangles);
	}
	std::shared_ptr<Mesh> CreateSphereMeshD(int smooth)
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		// add first part vertices
		float sqr3 = sqrtf(3), sqr5 = sqrtf(5);
		vertices.push_back(Point(1, sqr3, (3 + sqr5) / 2));
		vertices.push_back(Point(-2, 0, (3 + sqr5) / 2));
		vertices.push_back(Point(1, -sqr3, (3 + sqr5) / 2));
		vertices.push_back(Point(-(1 + sqr5) / 2, -(1 + sqr5) * sqr3 / 2, (sqr5 - 1) / 2));
		vertices.push_back(Point(1 + sqr5, 0, (sqr5 - 1) / 2));
		vertices.push_back(Point(-(1 + sqr5) / 2, (1 + sqr5) * sqr3 / 2, (sqr5 - 1) / 2));
		vertices.push_back(Point((1 + sqr5) / 2, (1 + sqr5) * sqr3 / 2, -(sqr5 - 1) / 2));
		vertices.push_back(Point(-(1 + sqr5), 0, -(sqr5 - 1) / 2));
		vertices.push_back(Point((1 + sqr5) / 2, -(1 + sqr5) * sqr3 / 2, -(sqr5 - 1) / 2));
		vertices.push_back(Point(-1, -sqr3, -(3 + sqr5) / 2));
		vertices.push_back(Point(2, 0, -(3 + sqr5) / 2));
		vertices.push_back(Point(-1, sqr3, -(3 + sqr5) / 2));
		for (auto& v : vertices)
		{
			float length = VectorLength(v.p);
			if (length != 0)
				v.p /= length;
			v.p.w = 1;
		}
		// add second part vertices
		SphereSkeleton(vertices, smooth, 0, 1); SphereSkeleton(vertices, smooth, 0, 2);
		SphereSkeleton(vertices, smooth, 0, 4); SphereSkeleton(vertices, smooth, 0, 6);
		SphereSkeleton(vertices, smooth, 0, 5); SphereSkeleton(vertices, smooth, 1, 2);
		SphereSkeleton(vertices, smooth, 2, 4); SphereSkeleton(vertices, smooth, 4, 6);
		SphereSkeleton(vertices, smooth, 6, 5); SphereSkeleton(vertices, smooth, 5, 1);
		// 1 2 4 6 5
		SphereSkeleton(vertices, smooth, 1, 7); SphereSkeleton(vertices, smooth, 1, 3);
		SphereSkeleton(vertices, smooth, 2, 3); SphereSkeleton(vertices, smooth, 2, 8);
		SphereSkeleton(vertices, smooth, 4, 8); SphereSkeleton(vertices, smooth, 4, 10);
		SphereSkeleton(vertices, smooth, 6, 10); SphereSkeleton(vertices, smooth, 6, 11);
		SphereSkeleton(vertices, smooth, 5, 11); SphereSkeleton(vertices, smooth, 5, 7);
		// 7 3 8 10 11
		SphereSkeleton(vertices, smooth, 7, 3); SphereSkeleton(vertices, smooth, 3, 8);
		SphereSkeleton(vertices, smooth, 8, 10); SphereSkeleton(vertices, smooth, 10, 11);
		SphereSkeleton(vertices, smooth, 11, 7); SphereSkeleton(vertices, smooth, 7, 9);
		SphereSkeleton(vertices, smooth, 3, 9); SphereSkeleton(vertices, smooth, 8, 9);
		SphereSkeleton(vertices, smooth, 10, 9); SphereSkeleton(vertices, smooth, 11, 9);
		// add triangles
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 0, 1, 2, +1, +6, -2), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 0, 2, 4, +2, +7, -3), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 0, 4, 6, +3, +8, -4), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 0, 6, 5, +4, +9, -5), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 0, 5, 1, +5, +10, -1), true));
		// 1 2 4 6 5
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 1, 7, 3, +11, +21, -12), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 1, 3, 2, +12, -13, -6), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 2, 3, 8, +13, +22, -14), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 2, 8, 4, +14, -15, -7), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 4, 8, 10, +15, +23, -16), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 4, 10, 6, +16, -17, -8), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 6, 10, 11, +17, +24, -18), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 6, 11, 5, +18, -19, -9), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 5, 11, 7, +19, +25, -20), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 5, 7, 1, +20, -11, -10), true));
		// 7 3 8 10 11
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 9, 3, 7, -27, -21, +26), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 9, 8, 3, -28, -22, +27), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 9, 10, 8, -29, -23, +28), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 9, 11, 10, -30, -24, +29), true));
		TrianglesAppend(triangles, BigTriangleLerp(vertices, SphereBigTriangle(smooth, 12, 9, 7, 11, -26, -25, +30), true));

		return std::make_shared<Mesh>(vertices, triangles);
	}
	void AddObject(std::shared_ptr<Object> pobj)
	{
		objs.push_back(pobj);
	}
	bool RemoveObject(std::shared_ptr<Object> pobj)
	{
		for (auto it = objs.begin(); it != objs.end(); it++)
		{
			if (*it == pobj)
			{
				it = objs.emplace(it);
				return true;
			}
		}
		return false;
	}
	std::shared_ptr<Object> GetObject(std::shared_ptr<Object> prev)
	{
		if (prev == nullptr)
			return objs.front();
		for (auto it = objs.begin(); it != objs.end(); it++)
		{
			if (*it == prev)
			{
				if (it + 1 == objs.end())
					return nullptr;
				else
					return *(it + 1);
			}
		}
		return nullptr;
	}
	Drawer::Drawer(uint* _buffer, int _width, int _height)
		: buffer(_buffer), w(_width), h(_height)
	{
	}
	Drawer::~Drawer()
	{
	}
	uint Drawer::Color(uchar r, uchar g, uchar b)
	{
		return (static_cast<uint>(r) << 16) | (static_cast<uint>(g) << 8) | (static_cast<uint>(b) << 0);
	}
	void Drawer::Pixel(Point2I p, uint color)
	{
		//if (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h)
		int i = p.y * w + p.x;
		buffer[i] = color;
	}
	void Drawer::Line(Point2I p1, Point2I p2, uint color)
	{
		if (p1.x == p2.x && p1.y == p2.y)
			Pixel(p1, color);
		else if (p1.y == p2.y)
		{
			if (p2.x < p1.x)
				Swap(p1, p2);
			for (int x = p1.x; x <= p2.x; x++)
				Pixel(Point2I(x, p1.y), color);
		}
		else if (p1.x == p2.x)
		{
			if (p2.y < p1.y)
				Swap(p1, p2);
			for (int y = p1.y; y <= p2.y; y++)
				Pixel(Point2I(p1.x, y), color);
		}
		else
		{
			int dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					Swap(p1, p2);
				int y_move = (p2.y >= p1.y) ? 1 : -1;
				int y_offset = dx / 2; // dx times of 1/2
				for (auto p = p1; p.x <= p2.x; p.x++)
				{
					Pixel(p, color);
					y_offset += dy;     // dx times of dy/dx
					if (y_offset >= dx) // dx times of >= 1
					{
						y_offset -= dx; // dx times of -= 1
						p.y += y_move;
					}
				}
			}
			else
			{
				if (p2.y < p1.y)
					Swap(p1, p2);
				int x_move = (p2.x >= p1.x) ? 1 : -1;
				int x_offset = dy / 2;
				for (auto p = p1; p.y <= p2.y; p.y++)
				{
					Pixel(p, color);
					x_offset += dx;
					if (x_offset >= dy)
					{
						x_offset -= dy;
						p.x += x_move;
					}
				}
			}
		}
	}
	DrawerZ::DrawerZ(uint* _buffer, int _width, int _height, float* _zbuffer)
		: buffer(_buffer), w(_width), h(_height), zbuffer(_zbuffer)
	{
	}
	DrawerZ::~DrawerZ()
	{
	}
	uint DrawerZ::Color(uchar r, uchar g, uchar b)
	{
		return Drawer::Color(r, g, b);
	}
	void DrawerZ::Pixel(Point2I p, uint color, float z)
	{
		//if (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h && z >= 0 && z <= 1)
		int i = p.y * w + p.x;
		if (z < zbuffer[i])
		{
			buffer[i] = color;
			zbuffer[i] = z;
		}
	}
	void DrawerZ::Line(Point2I p1, Point2I p2, uint color, float z1, float z2)
	{
		if (p1.x == p2.x && p1.y == p2.y)
			Pixel(p1, color, Min(z1, z2));
		else if (p1.y == p2.y)
		{
			if (p2.x < p1.x)
				Swap(p1, p2), Swap(z1, z2);
			for (int x = p1.x; x <= p2.x; x++)
			{
				float t = static_cast<float>(x - p1.x) / (p2.x - p1.x);
				Pixel(Point2I(x, p1.y), color, Lerp(z1, z2, t));
			}
		}
		else if (p1.x == p2.x)
		{
			if (p2.y < p1.y)
				Swap(p1, p2), Swap(z1, z2);
			for (int y = p1.y; y <= p2.y; y++)
			{
				float t = static_cast<float>(y - p1.y) / (p2.y - p1.y);
				Pixel(Point2I(p1.x, y), color, Lerp(z1, z2, t));
			}
		}
		else
		{
			int dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					Swap(p1, p2), Swap(z1, z2);
				for (auto p = p1; p.x <= p2.x; p.x++)
				{
					float t = static_cast<float>(p.x - p1.x) / (p2.x - p1.x);
					p.y = Lerp(p1.y, p2.y, t);
					Pixel(p, color, Lerp(z1, z2, t));
				}
			}
			else
			{
				if (p2.y < p1.y)
					Swap(p1, p2), Swap(z1, z2);
				for (auto p = p1; p.y <= p2.y; p.y++)
				{
					float t = static_cast<float>(p.y - p1.y) / (p2.y - p1.y);
					p.x = Lerp(p1.x, p2.x, t);
					Pixel(p, color, Lerp(z1, z2, t));
				}
			}
		}
	}
}
