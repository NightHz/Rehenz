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
	Camera::Camera() : position(), at(0, 0, 1, 0), up(0, 1, 0, 0)
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
	Camera::Camera(int _height, int _width) : position(), at(0, 0, 1, 0), up(0, 1, 0, 0)
	{
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];
		fovy = pi / 2;
		aspect = 4.0f / 3;
		z_near = 1;
		z_far = 500;
	}
	Camera::~Camera()
	{
		delete[] buffer;
	}
	void Camera::SetSize(int _height, int _width, float _fovy)
	{
		delete[] buffer;
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];
		fovy = _fovy;
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

		triangles.push_back(0); triangles.push_back(1); triangles.push_back(2);
		triangles.push_back(2); triangles.push_back(1); triangles.push_back(3);
		triangles.push_back(0); triangles.push_back(4); triangles.push_back(1);
		triangles.push_back(1); triangles.push_back(4); triangles.push_back(5);
		triangles.push_back(0); triangles.push_back(2); triangles.push_back(4);
		triangles.push_back(4); triangles.push_back(2); triangles.push_back(6);
		triangles.push_back(7); triangles.push_back(5); triangles.push_back(6);
		triangles.push_back(6); triangles.push_back(5); triangles.push_back(4);
		triangles.push_back(7); triangles.push_back(6); triangles.push_back(3);
		triangles.push_back(3); triangles.push_back(6); triangles.push_back(2);
		triangles.push_back(7); triangles.push_back(3); triangles.push_back(5);
		triangles.push_back(5); triangles.push_back(3); triangles.push_back(1);

		return std::make_shared<Mesh>(vertices, triangles);
	}
	std::shared_ptr<Mesh> SmoothSphereMesh(std::shared_ptr<Mesh> old)
	{
		auto v_old = old->GetVertices();
		auto tri_old = old->GetTriangles();
		int v_n = old->VertexCount();
		int tri_n = old->TriangleCount();

		std::vector<Vertex> vertices(v_old);
		std::vector<int> triangles;

		for (int i = 0; i < tri_n; i++)
		{
			// get three vertex index
			int a = tri_old[3 * i], b = tri_old[3 * i + 1], c = tri_old[3 * i + 2];
			// compute center vertex on unit sphere
			Point center = (v_old[a].p + v_old[b].p + v_old[c].p) / 3;
			float length = VectorLength(center);
			if (length != 0)
				center /= length;
			center.w = 1;
			// add new vertex
			int d = vertices.size();
			vertices.push_back(center);
			// add new triangles
			triangles.push_back(a); triangles.push_back(b); triangles.push_back(d);
			triangles.push_back(b); triangles.push_back(c); triangles.push_back(d);
			triangles.push_back(c); triangles.push_back(a); triangles.push_back(d);
		}

		return std::make_shared<Mesh>(vertices, triangles);
	}
	std::shared_ptr<Mesh> CreateSphereMesh(int smooth)
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		vertices.push_back(Point(0, 0, 1, 1));
		vertices.push_back(Point(1, 0, 0, 1));
		vertices.push_back(Point(0, 1, 0, 1));
		vertices.push_back(Point(-1, 0, 0, 1));
		vertices.push_back(Point(0, -1, 0, 1));
		vertices.push_back(Point(0, 0, -1, 1));

		triangles.push_back(0); triangles.push_back(2); triangles.push_back(1);
		triangles.push_back(0); triangles.push_back(3); triangles.push_back(2);
		triangles.push_back(0); triangles.push_back(4); triangles.push_back(3);
		triangles.push_back(0); triangles.push_back(1); triangles.push_back(4);
		triangles.push_back(5); triangles.push_back(1); triangles.push_back(2);
		triangles.push_back(5); triangles.push_back(2); triangles.push_back(3);
		triangles.push_back(5); triangles.push_back(3); triangles.push_back(4);
		triangles.push_back(5); triangles.push_back(4); triangles.push_back(1);

		auto pmesh = std::make_shared<Mesh>(vertices, triangles);

		for (; smooth > 0; smooth--)
			pmesh = SmoothSphereMesh(pmesh);

		return pmesh;
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
}
