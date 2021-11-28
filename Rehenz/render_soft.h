#pragma once
#include "math.h"
#include <vector>
#include <memory>
#include "type.h"
#include "mesh.h"

namespace Rehenz
{
	class Object;
	class Camera;

	class Objects;



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

	class Objects
	{
	private:
		// save all objects to render
		std::vector<std::shared_ptr<Object>> objs;

	public:
		Objects();
		~Objects();

		void AddObject(std::shared_ptr<Object> pobj);
		bool RemoveObject(std::shared_ptr<Object> pobj);
		// use nullptr get first object, and input last object will return nullptr
		std::shared_ptr<Object> GetObject(std::shared_ptr<Object> prev);

		static Objects global_objs;
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
		enum class RenderMode { Wireframe, PureWhite, Color };
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

		void SetSize(int _height, int _width, float _fovy);

		const uint* RenderImage(Objects& objs = Objects::global_objs);
	};
}