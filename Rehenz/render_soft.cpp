#include "render_soft.h"

namespace Rehenz
{
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
	Object::Object() : pmesh(nullptr), position(), at(), up(), scale()
	{
	}
	Object::Object(std::shared_ptr<Mesh> _pmesh) : pmesh(_pmesh), position(), at(), up(), scale()
	{
	}
	Object::~Object()
	{
	}
	void Object::SetMesh(std::shared_ptr<Mesh> _pmesh)
	{
		pmesh = _pmesh;
	}
}
