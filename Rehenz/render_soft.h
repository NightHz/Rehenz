#pragma once
#include "type.h"
#include "math.h"
#include <vector>
#include <memory>
#include "mesh.h"

namespace Rehenz
{
	class Transform;
	class Projection;

	class RenderObject;
	class Camera;

	class RenderObjects;



	class Transform
	{
	public:
		Vector pos;
		AircraftAxes axes;
		Vector scale;

		Transform();
		~Transform();

		Matrix GetTransformMatrix();
		Matrix GetInverseTransformMatrix();

		Vector GetFront();
		Vector GetUp();
		Vector GetRight();
		Vector GetFrontInGround();
		Vector GetRightInGround();

		void SetFront(Vector front);
	};

	class Projection
	{
	public:
		float fovy, aspect, z_near, z_far;

		// default fovy = pi/2, aspect = 1, z_near = 1, z_far = 500
		Projection();
		~Projection();

		Matrix GetTransformMatrix();

		Point GetOrigin();
	};

	class RenderObject
	{
	private:
	public:
		Transform transform;

		std::shared_ptr<Mesh> pmesh;

		std::shared_ptr<Texture> texture;
		std::shared_ptr<Texture> texture2;

		explicit RenderObject(std::shared_ptr<Mesh> _pmesh = nullptr,
			std::shared_ptr<Texture> _pt = nullptr, std::shared_ptr<Texture> _pt2 = nullptr);
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
		Transform transform;
		Projection projection;

		enum class RenderMode { Wireframe, PureWhite, Color, Texture, Shader };
		RenderMode render_mode;

		// default pos = (0,0,-5)
		explicit Camera(int _height, int _width);
		Camera(const Camera& c);
		Camera& operator=(const Camera& c) = delete;
		~Camera();

		inline int GetHeight() { return height; }
		inline int GetWidth() { return width; }
		inline const uint* GetLastImage() { return buffer; }

		void SetSize(int _height, int _width);

		const uint* RenderImage(RenderObjects& objs, VertexShader vertex_shader, PixelShader pixel_shader);
		inline const uint* RenderImage()
		{
			return RenderImage(RenderObjects::global_objs, DefaultVertexShader, DefaultPixelShader);
		}
		inline const uint* RenderImage(RenderObjects& objs)
		{
			return RenderImage(objs, DefaultVertexShader, DefaultPixelShader);
		}
		inline const uint* RenderImage(VertexShader vertex_shader, PixelShader pixel_shader)
		{
			return RenderImage(RenderObjects::global_objs, vertex_shader, pixel_shader);
		}
	};
}