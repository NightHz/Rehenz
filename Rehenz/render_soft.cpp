#include "render_soft.h"
#include "drawer.h"
#include "clipper.h"
#include <algorithm>

namespace Rehenz
{
	// Core Function
	const uint* Camera::RenderImage(RenderScene& scene, VertexShader vertex_shader, PixelShader pixel_shader)
	{
		// prepare drawer
		int size = height * width;
		float* zbuffer = new float[size];
		std::fill(zbuffer, zbuffer + size, projection.z_far + 1);
		std::fill(buffer, buffer + size, 0U);
		DrawerZ drawer(buffer, width, height, zbuffer);
		// prepare shader data
		VertexShaderData vshader_data;
		PixelShaderData pshader_data;
		vshader_data.mat_view = transform.GetInverseTransformMatrix();
		vshader_data.mat_project = projection.GetTransformMatrix();
		// traverse objects
		for (auto pobj = scene.GetObject(); pobj; pobj = scene.GetObject(pobj))
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
			std::vector<Point2I> screen_pos;
			for (auto& v : vertices)
			{
				// (-1,-1) -> (0,h), (1,1) -> (w,0)
				v *= 1 / v.p.w;
				int x = static_cast<int>((v.p.x + 1) * width / 2);
				int y = static_cast<int>((-v.p.y + 1) * height / 2);
				x = Clamp(x, 0, width - 1);
				y = Clamp(y, 0, height - 1);
				screen_pos.push_back(Point2I(x, y));
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
				Point2I pa = screen_pos[a], pb = screen_pos[b], pc = screen_pos[c];
				if (render_mode == RenderMode::Wireframe)
				{
					drawer.Line(pa, pb, drawer.ColorRGB(1.0f, 1.0f, 1.0f));
					drawer.Line(pa, pc, drawer.ColorRGB(1.0f, 1.0f, 1.0f));
					drawer.Line(pb, pc, drawer.ColorRGB(1.0f, 1.0f, 1.0f));
				}
				else if (render_mode == RenderMode::PureWhite)
				{
					drawer.Triangle(pa, pb, pc, drawer.ColorRGB(1.0f, 1.0f, 1.0f));
				}
				else if (render_mode == RenderMode::Color)
				{
					drawer.Triangle(pa, pb, pc, &va, &vb, &vc, DefaultPixelShader, pshader_data);
				}
				else if (render_mode == RenderMode::Texture)
				{
					drawer.Triangle(pa, pb, pc, &va, &vb, &vc, TexturePixelShader, pshader_data);
				}
				else if (render_mode == RenderMode::Shader)
				{
					drawer.Triangle(pa, pb, pc, &va, &vb, &vc, pixel_shader, pshader_data);
				}
			}
		}
		delete[] zbuffer;

		return buffer;
	}



	RenderObject::RenderObject(std::shared_ptr<Mesh> _pmesh, std::shared_ptr<Texture> _pt, std::shared_ptr<Texture> _pt2)
		: pmesh(_pmesh), texture(_pt), texture2(_pt2)
	{
	}

	RenderObject::~RenderObject()
	{
	}

	Camera::Camera(int _height, int _width) : render_mode(RenderMode::Wireframe)
	{
		height = _height;
		width = _width;
		int size = height * width;
		buffer = new uint[size];

		transform.pos = Vector(0, 0, -5);
		projection.aspect = static_cast<float>(width) / height;
	}

	Camera::Camera(const Camera& c) : transform(c.transform), projection(c.projection), render_mode(c.render_mode)
	{
		height = c.height;
		width = c.width;
		int size = height * width;
		buffer = new uint[size];
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

	void RenderScene::AddObject(std::shared_ptr<RenderObject> pobj)
	{
		objs.push_back(pobj);
	}

	bool RenderScene::RemoveObject(std::shared_ptr<RenderObject> pobj)
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

	RenderScene::obj_reader RenderScene::GetObject()
	{
		if (objs.empty())
			return obj_reader();
		obj_reader p;
		p.index = 0;
		p.pobj = objs[0].get();
		return p;
	}

	RenderScene::obj_reader RenderScene::GetObject(obj_reader prev)
	{
		if (!prev)
			return prev;
		if (prev.index + 1 >= objs.size())
			return obj_reader();
		obj_reader p;
		p.index = prev.index + 1;
		p.pobj = objs[p.index].get();
		return p;
	}

	Transform::Transform() : pos(0, 0, 0), axes(0, 0, 0), scale(1, 1, 1)
	{
	}

	Transform::~Transform()
	{
	}

	Matrix Transform::GetTransformMatrix()
	{
		return GetMatrixS(scale) * GetMatrixA(axes) * GetMatrixT(pos);
	}

	Matrix Transform::GetInverseTransformMatrix()
	{
		return  GetInverseMatrixT(pos) * GetInverseMatrixA(axes) * GetInverseMatrixS(scale);
	}

	Vector Transform::GetFront()
	{
		return Vector(0, 0, 1) * GetMatrixA(axes);
	}

	Vector Transform::GetUp()
	{
		return Vector(0, 1, 0) * GetMatrixA(axes);
	}

	Vector Transform::GetRight()
	{
		return Vector(1, 0, 0) * GetMatrixA(axes);
	}

	Vector Transform::GetFrontInGround()
	{
		Vector front = GetFront();
		float l2 = front.x * front.x + front.z * front.z;
		if (l2 < 0.0001f)
			return Vector(0, 0, 0);
		else
		{
			float divl = 1 / std::sqrtf(l2);
			return Vector(front.x * divl, 0, front.z * divl);
		}
	}

	Vector Transform::GetRightInGround()
	{
		Vector right = GetRight();
		float l2 = right.x * right.x + right.z * right.z;
		if (l2 < 0.0001f)
			return Vector(0, 0, 0);
		else
		{
			float divl = 1 / std::sqrtf(l2);
			return Vector(right.x * divl, 0, right.z * divl);
		}
	}

	void Transform::SetFront(Vector front)
	{
		float r = std::sqrtf(front.x * front.x + front.z * front.z);
		if (r == 0)
			axes.yaw = 0;
		else
			axes.yaw = std::asinf(front.x / r);
		if (front.y == 0)
			axes.pitch = 0;
		else if (r == 0)
			axes.pitch = (front.y > 0 ? -pi / 2 : pi / 2);
		else
			axes.pitch = -atanf(front.y / r);
	}

	Projection::Projection() :fovy(pi / 2), aspect(1), z_near(1), z_far(500)
	{
	}

	Projection::~Projection()
	{
	}

	Matrix Projection::GetTransformMatrix()
	{
		return GetMatrixP(fovy, aspect, z_near, z_far);
	}

	Point Projection::GetOrigin()
	{
		return Rehenz::GetOriginP(z_near, z_far);
	}

}
