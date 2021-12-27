#pragma once
#include "math.h"
#include <vector>
#include <memory>
#include "type.h"
#include "mesh.h"
#include <functional>

namespace Rehenz
{
	class RenderObject;
	class Camera;

	class RenderObjects;

	struct VertexShaderData;
	struct PixelShaderData;
	typedef std::function<Vertex(const VertexShaderData&, const Vertex&)> VertexShader;
	typedef std::function<Color(const PixelShaderData&, const Vertex&)> PixelShader;



	class RenderObject
	{
	private:
	public:
		std::shared_ptr<Mesh> pmesh;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Texture> texture2;
		Vector position;
		EulerAngles rotation;
		Vector scale;

		RenderObject();
		explicit RenderObject(std::shared_ptr<Mesh> _pmesh);
		explicit RenderObject(std::shared_ptr<Mesh> _pmesh, std::shared_ptr<Texture> _pt, std::shared_ptr<Texture> _pt2 = nullptr);
		~RenderObject();
	};

	void AddObject(std::shared_ptr<RenderObject> pobj);
	bool RemoveObject(std::shared_ptr<RenderObject> pobj);
	// use nullptr get first object, and input last object will return nullptr
	std::shared_ptr<RenderObject> GetObject(std::shared_ptr<RenderObject> prev);

	class RenderObjects
	{
	private:
		// save all objects to render
		std::vector<std::shared_ptr<RenderObject>> objs;

	public:
		RenderObjects();
		~RenderObjects();

		void AddObject(std::shared_ptr<RenderObject> pobj);
		bool RemoveObject(std::shared_ptr<RenderObject> pobj);
		// use nullptr get first object, and input last object will return nullptr
		std::shared_ptr<RenderObject> GetObject(std::shared_ptr<RenderObject> prev);

		static RenderObjects global_objs;
	};

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
		enum class RenderMode { Wireframe, PureWhite, Color, Texture, Shader };
		RenderMode render_mode;

		static VertexShader DefaultVertexShader;
		static PixelShader DefaultPixelShader;
		static PixelShader TexturePixelShader;

		// default height = 600, width = 800, fovy = pi/2, aspect = 4/3, z_near = 1, z_far = 500
		//         position = (0,0,-5), at = (0,0,1), up = (0,1,0)
		Camera();
		Camera(const Camera& c);
		Camera& operator=(const Camera& c) = delete;
		// auto set aspect = width / height
		explicit Camera(int _height, int _width);
		~Camera();

		int GetHeight() { return height; }
		int GetWidth() { return width; }
		const uint* GetLastImage() { return buffer; }

		void SetSize(int _height, int _width, float _aspect);

		const uint* RenderImage(RenderObjects& objs = RenderObjects::global_objs,
			VertexShader vertex_shader = DefaultVertexShader, PixelShader pixel_shader = DefaultPixelShader);
		const uint* RenderImage(VertexShader vertex_shader, PixelShader pixel_shader = DefaultPixelShader);
	};

	struct VertexShaderData
	{
	public:
		std::shared_ptr<RenderObject> pobj;
		Matrix mat_world;
		Matrix mat_view;
		Matrix mat_project;
		// = mat_world * mat_view * mat_project
		Matrix transform;
	};

	struct PixelShaderData
	{
	public:
		std::shared_ptr<RenderObject> pobj;
	};
}