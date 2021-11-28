#include "render_soft.h"
#include "drawer.h"

namespace Rehenz
{
	// Core Function
	const uint* Camera::RenderImage(Objects& objs)
	{
		// Copy and transform all objects (vertex shader)
		std::vector<Vertex> vertices;
		std::vector<int> triangles;
		Matrix mat_view = GetInverseMatrixT(position) * GetInverseMatrixR(at, up) * GetMatrixP(fovy, aspect, z_near, z_far);
		for (auto pobj = objs.GetObject(nullptr); pobj != nullptr; pobj = objs.GetObject(pobj))
		{
			Matrix mat_world = GetMatrixS(pobj->scale) * GetMatrixE(pobj->rotation) * GetMatrixT(pobj->position);
			Matrix transform = mat_world * mat_view;
			auto& vs_mesh = pobj->pmesh->GetVertices();
			auto& tris_mesh = pobj->pmesh->GetTriangles();
			int v_index_offset = static_cast<int>(vertices.size());
			for (auto v : vs_mesh)
			{
				v.p = PointStandard(v.p * transform);
				vertices.push_back(v);
			}
			for (auto tri : tris_mesh)
				triangles.push_back(tri + v_index_offset);
		}

		// Clipping and back-face culling
		std::vector<int> tris_new;
		for (size_t i = 0; i < triangles.size(); i += 3)
		{
			int a = triangles[i], b = triangles[i + 1], c = triangles[i + 2];
			Vertex& va = vertices[a], & vb = vertices[b], & vc = vertices[c];
			static auto Inside = [](Vertex& v) -> bool { return v.p.x >= -1 && v.p.x <= 1 && v.p.y >= -1 && v.p.y <= 1 && v.p.z >= 0 && v.p.z <= 1; };
			if (Inside(va) && Inside(vb) && Inside(vc) && TrianglesNormal(va.p, vb.p, vc.p).z < 0)
			{
				tris_new.push_back(a); tris_new.push_back(b); tris_new.push_back(c);
			}
		}
		triangles.swap(tris_new);
		tris_new.clear();

		// Mapping to screen
		std::vector<Point2I> screen_pos;
		for (auto& v : vertices)
		{
			// (-1,-1) -> (0,h-1), (1,1) -> (w-1,0)
			screen_pos.push_back(Point2I(static_cast<int>((v.p.x + 1) * (width - 1) / 2), static_cast<int>((-v.p.y + 1) * (height - 1) / 2)));
		}

		// Traverse all triangles and compute color for all sampling points (pixel shader)
		int size = height * width;
		float* zbuffer = new float[size];
		std::fill(zbuffer, zbuffer + size, 2.0f);
		std::fill(buffer, buffer + size, 0U);
		DrawerZ drawer(buffer, width, height, zbuffer);
		for (size_t i = 0; i < triangles.size(); i += 3)
		{
			int a = triangles[i], b = triangles[i + 1], c = triangles[i + 2];
			Vertex& va = vertices[a], & vb = vertices[b], & vc = vertices[c];
			Point2I pa = screen_pos[a], pb = screen_pos[b], pc = screen_pos[c];
			// Use z-buffer merge multiple colors
			if (render_mode == RenderMode::Wireframe)
			{
				drawer.Line(pa, pb, drawer.Color(1.0f, 1.0f, 1.0f), va.p.z, vb.p.z);
				drawer.Line(pa, pc, drawer.Color(1.0f, 1.0f, 1.0f), va.p.z, vc.p.z);
				drawer.Line(pb, pc, drawer.Color(1.0f, 1.0f, 1.0f), vb.p.z, vc.p.z);
			}
			else if (render_mode == RenderMode::PureWhite)
			{
				drawer.Triangle(pa, pb, pc, drawer.Color(1.0f, 1.0f, 1.0f), va.p.z, vb.p.z, vc.p.z);
			}
			else if (render_mode == RenderMode::Color)
			{
				drawer.Triangle(pa, pb, pc, &va, &vb, &vc);
			}
		}
		delete[] zbuffer;

		return buffer;
	}



	Object::Object() : pmesh(nullptr), texture(nullptr), texture2(nullptr), position(), rotation(), scale(1, 1, 1, 0)
	{
	}
	Object::Object(std::shared_ptr<Mesh> _pmesh) : pmesh(_pmesh), texture(nullptr), texture2(nullptr), position(), rotation(), scale(1, 1, 1, 0)
	{
	}
	Object::Object(std::shared_ptr<Mesh> _pmesh, std::shared_ptr<Texture> _pt, std::shared_ptr<Texture> _pt2)
		: pmesh(_pmesh), texture(_pt), texture2(_pt2), position(), rotation(), scale(1, 1, 1, 0)
	{
	}
	Object::~Object()
	{
	}
	Camera::Camera() : position(0, 0, -5, 0), at(0, 0, 1, 0), up(0, 1, 0, 0), render_mode(RenderMode::Wireframe)
	{
		height = 600;
		width = 800;
		int size = height * width;
		buffer = new uint[size];
		fovy = pi / 2;
		aspect = 4.0f / 3;
		z_near = 1;
		z_far = 500;
	}
	Camera::Camera(const Camera& c) : position(c.position), at(c.at), up(c.up), render_mode(c.render_mode)
	{
		height = c.height;
		width = c.width;
		int size = height * width;
		buffer = new uint[size];
		fovy = c.fovy;
		aspect = c.aspect;
		z_near = c.z_near;
		z_far = c.z_far;
	}
	Camera::Camera(int _height, int _width) : position(0, 0, -5, 0), at(0, 0, 1, 0), up(0, 1, 0, 0), render_mode(RenderMode::Wireframe)
	{
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];
		fovy = pi / 2;
		aspect = static_cast<float>(width) / height;
		z_near = 1;
		z_far = 500;
	}
	Camera::~Camera()
	{
		delete[] buffer;
	}
	void Camera::SetSize(int _height, int _width, float _aspect)
	{
		delete[] buffer;
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];
		aspect = _aspect;
	}



	Objects Objects::global_objs = Objects();
	Objects::Objects()
	{
	}
	Objects::~Objects()
	{
	}
	void AddObject(std::shared_ptr<Object> pobj)
	{
		Objects::global_objs.AddObject(pobj);
	}
	bool RemoveObject(std::shared_ptr<Object> pobj)
	{
		return Objects::global_objs.RemoveObject(pobj);
	}
	std::shared_ptr<Object> GetObject(std::shared_ptr<Object> prev)
	{
		return Objects::global_objs.GetObject(prev);
	}
	void Objects::AddObject(std::shared_ptr<Object> pobj)
	{
		objs.push_back(pobj);
	}
	bool Objects::RemoveObject(std::shared_ptr<Object> pobj)
	{
		for (auto it = objs.begin(); it != objs.end(); it++)
		{
			if (*it == pobj)
			{
				it = objs.emplace(it);
				return true;
			}
		}
		return false;
	}
	std::shared_ptr<Object> Objects::GetObject(std::shared_ptr<Object> prev)
	{
		if (prev == nullptr)
			return objs.front();
		for (auto it = objs.begin(); it != objs.end(); it++)
		{
			if (*it == prev)
			{
				if (it + 1 == objs.end())
					return nullptr;
				else
					return *(it + 1);
			}
		}
		return nullptr;
	}
}
