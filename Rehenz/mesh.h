#pragma once
#include "type.h"
#include "math.h"
#include "color.h"

namespace Rehenz
{
	// 2-component uv coordinates, u=x, v=y
	typedef Vector2 UV;

	struct Vertex;
	class Mesh;

	class Texture;

	struct ShaderData;
	typedef ShaderData VertexShaderData;
	typedef ShaderData PixelShaderData;
	typedef std::function<Vertex(const VertexShaderData&, const Vertex&)> VertexShader;
	typedef std::function<Color(const PixelShaderData&, const Vertex&)> PixelShader;



	struct Vertex
	{
	public:
		// position
		Point p;
		// normal
		Vector n;
		// color
		Color c;
		// texcoord
		UV uv;
		UV uv2;
		// default value is 1, save multiplication factor
		// when multiply by 1/w to standardize p, save 1/w
		float coef;

		Vertex();
		Vertex(Point _p);
		explicit Vertex(Point _p, Vector _n, Color _c = Color(1, 1, 1), UV _uv = UV(0, 0), UV _uv2 = UV(0, 0), float _coef = 1);
		explicit Vertex(Point _p, Color _c, UV _uv = UV(0, 0), UV _uv2 = UV(0, 0), float _coef = 1);
		explicit Vertex(Point _p, UV _uv, UV _uv2 = UV(0, 0), float _coef = 1);

		Vertex& operator*=(float);
		Vertex& operator+=(const Vertex&);

		Vertex operator*(float) const;
		Vertex operator+(const Vertex&) const;
		Vertex operator-(const Vertex&) const;
	};

	// divide by coef to recover vertex attributes except position
	Vertex VertexRecover(const Vertex& v);

	Vertex VertexLerp(const Vertex& v1, const Vertex& v2, float t);

	class Mesh
	{
	private:
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

	public:
		Mesh();
		explicit Mesh(const std::vector<Vertex>& _vertices, const std::vector<int>& _triangles);
		explicit Mesh(const std::vector<Vertex>&& _vertices, const std::vector<int>&& _triangles);
		~Mesh();

		inline size_t VertexCount() { return vertices.size(); }
		inline size_t TriangleCount() { return triangles.size() / 3; }
		inline size_t IndexCount() { return triangles.size(); }
		inline const std::vector<Vertex>& GetVertices() { return vertices; }
		inline const std::vector<int>& GetTriangles() { return triangles; }

		void AddVertex(Vertex vertex);
		void AddVertex(const std::vector<Vertex>& _vertices);
		void AddTriangle(int a, int b, int c);
		void AddTriangle(const std::vector<int>& _triangles);
	};

	// create cube mesh which includes pos, normal, color, uv, uv2 info
	// pos   : 2x2x2 cube, and (0,0,0) is center point
	// normal: face normal
	// color : use colors fill, and default is white
	// uv    : each faces map to the whole texture
	// uv2   : expanded faces map to the texture
	std::shared_ptr<Mesh> CreateCubeMesh(const std::vector<Color>& colors = std::vector<Color>(), int smooth = 2);

	// create a colorful cube mesh based function CreateCubeMesh
	inline std::shared_ptr<Mesh> CreateCubeMeshColorful(int smooth = 2)
	{
		const std::vector<Color> colors{ Color(0, 0, 0), Color(1, 0, 0), Color(0, 1, 0), Color(1, 1, 0),
										 Color(0, 0, 1), Color(1, 0, 1), Color(0, 1, 1), Color(1, 1, 1) };
		return CreateCubeMesh(colors, smooth);
	}

	// create sphere mesh
	// pos   : unit sphere, and (0,0,0) is center point
	// normal: point out of the ball through ball center
	// color : white
	// uv    : (u=phi/2pi, v=theta/pi)
	std::shared_ptr<Mesh> CreateSphereMesh(int smooth = 10);
	// another sphere mesh layout. I call it B layout. default is A layout
	// only uv is different with A layout : tile big triangles to define uv, be same with Net (polyhedron)
	std::shared_ptr<Mesh> CreateSphereMeshB(int smooth = 5);
	// sphere mesh C layout, uv same with B
	std::shared_ptr<Mesh> CreateSphereMeshC(int smooth = 8);
	// sphere mesh D layout, uv same with B
	std::shared_ptr<Mesh> CreateSphereMeshD(int smooth = 4);

	// create mesh from .obj file
	// pos   : from file
	// normal: from file
	// color : white
	// uv    : from file
	std::shared_ptr<Mesh> CreateMeshFromObjFile(const std::string& filename);

	// create frustum mesh which includes pos, normal info
	// pos   : height = 2, bottom radius = 1, (0,0,0) is center point
	// normal: 
	// color : white
	// uv    : topologically be same with sphere A
	std::shared_ptr<Mesh> CreateFrustumMesh(float top_radius = 0.5f, int smooth = 10);

	class Texture
	{
	private:
	public:
		const int width;
		const int height;
		Color* const buffer;

		Texture(int _width, int _height);
		Texture(const Texture& t);
		Texture& operator=(const Texture&) = delete;
		~Texture();

		Color GetColor(float u, float v);
		Color GetColor(UV uv);
	};

	// create digit 1 texture
	std::shared_ptr<Texture> CreateTexture1();
	// create letter C texture
	std::shared_ptr<Texture> CreateTextureC();
	// create white-blue plaid texture
	std::shared_ptr<Texture> CreateTexturePlaid(int n = 10);
	// create 1-6 dice texture
	std::shared_ptr<Texture> CreateTextureDice();

	struct ShaderData
	{
	public:
		Matrix mat_world;
		Matrix mat_inv_world;
		Matrix mat_view;
		Matrix mat_project;
		// = mat_world * mat_view * mat_project
		Matrix transform;

		std::shared_ptr<Texture> texture;
		std::shared_ptr<Texture> texture2;
	};
}