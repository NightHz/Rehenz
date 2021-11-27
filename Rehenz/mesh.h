#pragma once
#include "math.h"
#include <vector>
#include <memory>
#include "type.h"

namespace Rehenz
{
	struct Vertex;
	class Mesh;


	struct Vertex
	{
	public:
		Point p;

		Vertex(Point _p);
	};

	Vertex VertexLerp(const Vertex& v1, const Vertex& v2, float t);

	class Mesh
	{
	private:
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

	public:
		Mesh();
		explicit Mesh(const std::vector<Vertex>& _vertices, const std::vector<int>& _triangles);
		~Mesh();

		size_t VertexCount() { return vertices.size(); }
		size_t TriangleCount() { return triangles.size() / 3; }
		const std::vector<Vertex>& GetVertices() { return vertices; }
		const std::vector<int>& GetTriangles() { return triangles; }

		void AddVertex(Vertex vertex);
		void AddVertex(const std::vector<Vertex>& _vertices);
		void AddTriangle(int a, int b, int c);
		void AddTriangle(const std::vector<int>& _triangles);
	};

	std::shared_ptr<Mesh> CreateCubeMesh();
	std::shared_ptr<Mesh> CreateSphereMesh(int smooth = 10);
	// another mesh layout. I call it B layout. default is A layout
	std::shared_ptr<Mesh> CreateSphereMeshB(int smooth = 5);
	// C layout
	std::shared_ptr<Mesh> CreateSphereMeshC(int smooth = 8);
	// D layout
	std::shared_ptr<Mesh> CreateSphereMeshD(int smooth = 4);
}