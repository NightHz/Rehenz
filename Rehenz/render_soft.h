#pragma once
#include "math.h"
#include <vector>
#include <memory>

namespace Rehenz
{
	struct Vertex
	{
	public:
		Point p;

		Vertex(Point _p);
	};

	class Mesh
	{
	private:
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

	public:
		Mesh();
		explicit Mesh(const std::vector<Vertex>& _vertices, const std::vector<int>& _triangles);
		~Mesh();

		int VertexCount() { return vertices.size(); }
		int TriangleCount() { return triangles.size() / 3; }
		const std::vector<Vertex> GetVertices() { return vertices; }
		const std::vector<int> GetTriangles() { return triangles; }

		void AddVertex(Vertex vertex);
		void AddVertex(const std::vector<Vertex>& _vertices);
		void AddTriangle(int a, int b, int c);
		void AddTriangle(const std::vector<int>& _triangles);
	};

	class Object
	{
	private:
		std::shared_ptr<Mesh> pmesh;

	public:
		Vector position;
		// at vector for rotation
		Vector at;
		// up vector for rotation
		Vector up;
		Vector scale;

		Object();
		explicit Object(std::shared_ptr<Mesh> _pmesh);
		~Object();

		std::shared_ptr<Mesh> GetMesh() { return pmesh; }

		void SetMesh(std::shared_ptr<Mesh> _pmesh);
	};

	void AddObject(std::shared_ptr<Object> pobj);
	void RemoveObject(std::shared_ptr<Object> pobj);
	// use nullptr get first object, and input last object will return nullptr
	std::shared_ptr<Object> GetObject(std::shared_ptr<Object> prev);

	class Camera
	{
	private:
	public:
	};
}