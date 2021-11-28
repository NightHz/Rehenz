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
		// position
		Point p;
		// color
		Vector c;
		Vector2 uv;
		Vector2 uv2;
		Vector2 uv3;
		Vector2 uv4;

		Vertex(Point _p);
		explicit Vertex(Point _p, Vector _c,
			Vector2 _uv = Vector2(0, 0), Vector2 _uv2 = Vector2(0, 0), Vector2 _uv3 = Vector2(0, 0), Vector2 _uv4 = Vector2(0, 0));
		explicit Vertex(Point _p, Vector2 _uv,
			Vector2 _uv2 = Vector2(0, 0), Vector2 _uv3 = Vector2(0, 0), Vector2 _uv4 = Vector2(0, 0));
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

	// create cube mesh which includes pos, color, uv, uv2 info
	// pos   : unit cube, and (0,0,0) is center point
	// color : use colors fill, and default is white
	// uv    : each faces map to the whole texture
	// uv2   : expanded faces map to the texture
	std::shared_ptr<Mesh> CreateCubeMesh(const std::vector<Vector>& colors = std::vector<Vector>());

	// create sphere mesh which includes pos info
	// pos   : unit sphere, and (0,0,0) is center point
	std::shared_ptr<Mesh> CreateSphereMesh(int smooth = 10);
	// another sphere mesh layout. I call it B layout. default is A layout
	std::shared_ptr<Mesh> CreateSphereMeshB(int smooth = 5);
	// sphere mesh C layout
	std::shared_ptr<Mesh> CreateSphereMeshC(int smooth = 8);
	// sphere mesh D layout
	std::shared_ptr<Mesh> CreateSphereMeshD(int smooth = 4);
}