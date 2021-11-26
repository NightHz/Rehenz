#pragma once
#include "math.h"
#include <vector>
#include <memory>
#include "type.h"

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
	std::shared_ptr<Mesh> CreateSphereMesh(int smooth = 3);

	class Object
	{
	private:
	public:
		std::shared_ptr<Mesh> pmesh;
		Vector position;
		EulerAngles rotation;
		Vector scale;

		Object();
		explicit Object(std::shared_ptr<Mesh> _pmesh);
		~Object();
	};

	void AddObject(std::shared_ptr<Object> pobj);
	bool RemoveObject(std::shared_ptr<Object> pobj);
	// use nullptr get first object, and input last object will return nullptr
	std::shared_ptr<Object> GetObject(std::shared_ptr<Object> prev);

	class Camera
	{
	private:
		int height, width;
		// last buffer image
		uint* buffer;

	public:
		Vector position;
		Vector at;
		Vector up;
		// projection parameters
		float fovy, aspect, z_near, z_far;

		// default height = 600, width = 800, fovy = pi/2, aspect = 4/3, z_near = 1, z_far = 500
		//         position = (0,0,-5), at = (0,0,1), up = (0,1,0)
		Camera();
		Camera(const Camera& c) = delete;
		Camera& operator=(const Camera& c) = delete;
		// auto set aspect = width / height
		explicit Camera(int _height, int _width);
		~Camera();

		int GetHeight() { return height; }
		int GetWidth() { return width; }
		const uint* GetLastImage() { return buffer; }

		void SetSize(int _height, int _width, float _fovy);

		const uint* RenderImage();
	};

	class Drawer
	{
	private:
		uint* const buffer;
		const int w;
		const int h;

	public:
		Drawer(uint* _buffer, int _width, int _height);
		~Drawer();

		static uint Color(uchar r, uchar g, uchar b);
		void Pixel(Point2I p, uint color);
		void Line(Point2I p1, Point2I p2, uint color);
	};

	class DrawerZ
	{
	private:
		uint* const buffer;
		const int w;
		const int h;
		float* const zbuffer;

	public:
		DrawerZ(uint* _buffer, int _width, int _height, float* _zbuffer);
		~DrawerZ();

		static uint Color(uchar r, uchar g, uchar b);
		void Pixel(Point2I p, uint color, float z);
		void Line(Point2I p1, Point2I p2, uint color, float z1, float z2);
	};
}