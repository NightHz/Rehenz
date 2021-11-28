#include "render_soft.h"
#include "drawer.h"

namespace Rehenz
{
	VertexShader Camera::DefaultVertexShader = [](const VertexShaderData& data, const Vertex& v0)->Vertex
	{
		Vertex v(v0);
		v.p = PointStandard(v.p * data.transform);
		return v;
	};

	PixelShader Camera::DefaultPixelShader = [](const PixelShaderData& data, const Vertex& v0)->Vector
	{
		(data); // unreferenced
		return v0.c;
	};

	// Core Function
	const uint* Camera::RenderImage(Objects& objs, VertexShader vertex_shader, PixelShader pixel_shader)
	{
		VertexShaderData vshader_data;
		PixelShaderData pshader_data;
		vshader_data.mat_view = GetInverseMatrixT(position) * GetInverseMatrixR(at, up);
		vshader_data.mat_project = GetMatrixP(fovy, aspect, z_near, z_far);
		int size = height * width;
		float* zbuffer = new float[size];
		std::fill(zbuffer, zbuffer + size, 2.0f);
		std::fill(buffer, buffer + size, 0U);
		DrawerZ drawer(buffer, width, height, zbuffer);
		for (auto pobj = objs.GetObject(nullptr); pobj != nullptr; pobj = objs.GetObject(pobj))
		{
			// Copy and transform vertices (vertex shader)
			vshader_data.pobj = pobj;
			vshader_data.mat_world = GetMatrixS(pobj->scale) * GetMatrixE(pobj->rotation) * GetMatrixT(pobj->position);
			vshader_data.transform = vshader_data.mat_world * vshader_data.mat_view * vshader_data.mat_project;
			auto& vs_mesh = pobj->pmesh->GetVertices();
			std::vector<Vertex> vertices;
			for (auto& v : vs_mesh)
			{
				vertices.push_back(vertex_shader(vshader_data, v));
			}

			// Clipping and back-face culling
			static auto Inside = [](Vertex& v) -> bool { return v.p.x >= -1 && v.p.x <= 1 && v.p.y >= -1 && v.p.y <= 1 && v.p.z >= 0 && v.p.z <= 1; };
			auto& tris_mesh = pobj->pmesh->GetTriangles();
			std::vector<int> triangles;
			for (size_t i = 0; i < tris_mesh.size(); i += 3)
			{
				int a = tris_mesh[i], b = tris_mesh[i + 1], c = tris_mesh[i + 2];
				Vertex& va = vertices[a], & vb = vertices[b], & vc = vertices[c];
				if (TrianglesNormal(va.p, vb.p, vc.p).z < 0)
				{
					if (Inside(va) && Inside(vb) && Inside(vc))
					{
						triangles.push_back(a); triangles.push_back(b); triangles.push_back(c);
					}
				}
			}

			// Mapping to screen
			std::vector<Point2I> screen_pos;
			for (auto& v : vertices)
			{
				// (-1,-1) -> (0,h-1), (1,1) -> (w-1,0)
				screen_pos.push_back(Point2I(static_cast<int>((v.p.x + 1) * (width - 1) / 2), static_cast<int>((-v.p.y + 1) * (height - 1) / 2)));
			}

			// Traverse all triangles and compute color for all sampling points (pixel shader)
			pshader_data.pobj = pobj;
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
					drawer.Triangle(pa, pb, pc, &va, &vb, &vc, DefaultPixelShader, pshader_data);
				}
			}
		}
		delete[] zbuffer;

		return buffer;
	}

	const uint* Camera::RenderImage(VertexShader vertex_shader, PixelShader pixel_shader)
	{
		return RenderImage(Objects::global_objs, vertex_shader, pixel_shader);
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
