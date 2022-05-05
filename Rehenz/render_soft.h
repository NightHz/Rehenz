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