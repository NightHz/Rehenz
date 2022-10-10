#include "render_soft.h"
#include "drawer.h"
#include "clipper.h"
#include <algorithm>
#include "shader.h"

namespace Rehenz
{
	// Core Function
	const uint* Camera::RenderImage(RenderScene& scene)
	{
		// prepare drawer
		int size = height * width;
		auto zbuffer = std::make_unique<float[]>(size);
		std::fill(zbuffer.get(), zbuffer.get() + size, 1.0f);
		std::fill(buffer, buffer + size, 0U);
		DrawerV drawer(buffer, width, height, zbuffer.get());
		DrawerF drawerf(buffer, width, height);
		// prepare shader data
		VertexShaderData vshader_data;
		PixelShaderData pshader_data;
		vshader_data.mat_view = transform.GetInverseTransformMatrix();
		vshader_data.mat_project = projection.GetTransformMatrix();
		// traverse objects
		for (auto pobj = scene.GetRenderObject(); pobj; pobj = scene.GetRenderObject(pobj))
		{
			// Copy and transform vertices (vertex shader)
			vshader_data.mat_world = pobj->transform.GetTransformMatrix();
			vshader_data.transform = vshader_data.mat_world * vshader_data.mat_view * vshader_data.mat_project;
			auto& vs_mesh = pobj->pmesh->GetVertices();
			std::vector<Vertex> vertices;
			for (auto& v : vs_mesh)
			{
				vertices.push_back(vertex_shader(vshader_data, v));
			}

			// Clipping and back-face culling
			auto& tris_mesh = pobj->pmesh->GetTriangles();
			std::vector<int> triangles;
			Point origin = projection.GetOrigin();
			for (size_t i = 0; i < tris_mesh.size(); i += 3)
			{
				int a = tris_mesh[i], b = tris_mesh[i + 1], c = tris_mesh[i + 2];
				Vertex& va = vertices[a], & vb = vertices[b], & vc = vertices[c];

				auto sight = va.p - origin;
				auto normal = TrianglesNormal(va.p, vb.p, vc.p);
				auto dot_sight_normal = VectorDot(sight, normal);

				if (dot_sight_normal < 0) // judge back-face
				{
					if (ClipPointInside(va.p) && ClipPointInside(vb.p) && ClipPointInside(vc.p))
					{
						triangles.push_back(a); triangles.push_back(b); triangles.push_back(c);
					}
					else // clipping
					{
						ClipTriangleCohenSutherland(vertices, triangles, a, b, c);
					}
				}
			}

			// Mapping to screen
			for (auto& v : vertices)
			{
				// (-1,-1) -> (0,h), (1,1) -> (w,0)
				v *= 1 / v.p.w;
				v.p.x = (v.p.x + 1) * width / 2;
				v.p.y = (-v.p.y + 1) * height / 2;
			}

			// Traverse all triangles and sampling
			// Compute color for all sampling points (pixel shader)
			// Use z-buffer merge multiple colors
			pshader_data.texture = pobj->texture;
			pshader_data.texture2 = pobj->texture2;
			for (size_t i = 0; i < triangles.size(); i += 3)
			{
				int a = triangles[i], b = triangles[i + 1], c = triangles[i + 2];
				Vertex& va = vertices[a], & vb = vertices[b], & vc = vertices[c];
				Point& pa = va.p, & pb = vb.p, & pc = vc.p;
				if (render_mode == RenderMode::Wireframe)
				{
					drawerf.Line(pa, pb, drawerf.white);
					drawerf.Line(pa, pc, drawerf.white);
					drawerf.Line(pb, pc, drawerf.white);
				}
				else if (render_mode == RenderMode::PureWhite)
				{
					drawerf.Triangle(pa, pb, pc, drawerf.white);
				}
				/*else if (render_mode == RenderMode::FlatColor)
				{
					drawerf.Triangle(pa, pb, pc, drawerf.ColorRGB(VertexRecover(va).c));
				}*/
				else if (render_mode == RenderMode::Shader)
				{
					drawer.Triangle(va, vb, vc, pixel_shader, pshader_data);
				}
			}
		}

		return buffer;
	}



	RenderObject::RenderObject(std::shared_ptr<Mesh> _pmesh, std::shared_ptr<Texture> _pt, std::shared_ptr<Texture> _pt2)
		: pmesh(_pmesh), texture(_pt), texture2(_pt2)
	{
	}

	RenderObject::~RenderObject()
	{
	}

	Camera::Camera(int _height, int _width)
	{
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];

		transform.pos = Vector(0, 0, -5);
		projection.aspect = static_cast<float>(width) / height;

		render_mode = RenderMode::Wireframe;
		vertex_shader = DefaultVertexShader;
		pixel_shader = DefaultPixelShader;
	}

	Camera::Camera(const Camera& c) : transform(c.transform), projection(c.projection)
	{
		height = c.height;
		width = c.width;
		int size = height * width;
		buffer = new uint[size];

		render_mode = c.render_mode;
		vertex_shader = c.vertex_shader;
		pixel_shader = c.pixel_shader;
	}

	Camera::~Camera()
	{
		delete[] buffer;
	}

	void Camera::SetSize(int _height, int _width)
	{
		delete[] buffer;
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];
		
		projection.aspect = static_cast<float>(width) / height;
	}



	RenderScene RenderScene::global_scene = RenderScene();

	RenderScene::RenderScene()
	{
	}

	RenderScene::~RenderScene()
	{
	}

	void RenderScene::AddRenderObject(std::shared_ptr<RenderObject> pobj)
	{
		objs.push_back(pobj);
	}

	bool RenderScene::RemoveRenderObject(std::shared_ptr<RenderObject> pobj)
	{
		auto it = std::find(objs.begin(), objs.end(), pobj);
		if (it != objs.end())
		{
			*it = objs.back();
			objs.pop_back();
			return true;
		}
		else
			return false;
	}

	RenderScene::obj_reader RenderScene::GetRenderObject()
	{
		if (objs.empty())
			return obj_reader();
		obj_reader p;
		p.index = 0;
		p.pobj = objs[0].get();
		return p;
	}

	RenderScene::obj_reader RenderScene::GetRenderObject(obj_reader prev)
	{
		if (!prev)
			return prev;
		if (prev.index >= objs.size() - 1)
			return obj_reader();
		obj_reader p;
		p.index = prev.index + 1;
		p.pobj = objs[p.index].get();
		return p;
	}

}
