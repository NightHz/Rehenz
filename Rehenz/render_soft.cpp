#include "render_soft.h"

namespace Rehenz
{
	// save all objects to render
	std::vector<std::shared_ptr<Object>> objs;


	Vertex::Vertex(Point _p) : p(_p)
	{
	}

	Mesh::Mesh() : vertices(), triangles()
	{
	}
	Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<int>& _triangles) : vertices(_vertices), triangles(_triangles)
	{
	}
	Mesh::~Mesh()
	{
	}
	void Mesh::AddVertex(Vertex vertex)
	{
		vertices.push_back(vertex);
	}
	void Mesh::AddVertex(const std::vector<Vertex>& _vertices)
	{
		vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
	}
	void Mesh::AddTriangle(int a, int b, int c)
	{
		triangles.push_back(a);
		triangles.push_back(b);
		triangles.push_back(c);
	}
	void Mesh::AddTriangle(const std::vector<int>& _triangles)
	{
		triangles.insert(triangles.end(), _triangles.begin(), _triangles.end());
	}
	Object::Object() : pmesh(nullptr), position(), rotation(), scale(1, 1, 1, 0)
	{
	}
	Object::Object(std::shared_ptr<Mesh> _pmesh) : pmesh(_pmesh), position(), rotation(), scale(1, 1, 1, 0)
	{
	}
	Object::~Object()
	{
	}
	Camera::Camera() : position(0, 0, -5, 0), at(0, 0, 1, 0), up(0, 1, 0, 0)
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
	Camera::Camera(int _height, int _width) : position(0, 0, -5, 0), at(0, 0, 1, 0), up(0, 1, 0, 0)
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
	const uint* Camera::RenderImage()
	{
		// Copy and transform all objects (vertex shader)
		std::vector<Vertex> vertices;
		std::vector<int> triangles;
		Matrix mat_view = GetMatrixT(position) * GetMatrixR(at, up) * GetMatrixP(fovy, aspect, z_near, z_far);
		for (auto pobj : objs)
		{
			Matrix mat_world = GetMatrixS(pobj->scale) * GetMatrixE(pobj->rotation) * GetMatrixT(pobj->position);
			Matrix transform = mat_world * mat_view;
			auto& v_mesh = pobj->pmesh->GetVertices();
			auto& tri_mesh = pobj->pmesh->GetTriangles();
			int v_index_offset = static_cast<int>(vertices.size());
			for (auto& v : v_mesh)
				vertices.push_back(Vertex(PointStandard(v.p * transform)));
			for (auto tri : tri_mesh)
				triangles.push_back(tri + v_index_offset);
		}

		// Clipping and back-face culling
		std::vector<int> tri_new;
		for (size_t i = 0; i < triangles.size(); i += 3)
		{
			int a = triangles[i], b = triangles[i + 1], c = triangles[i + 2];
			Vertex& va = vertices[a], vb = vertices[b], vc = vertices[c];
			auto Inside = [](Vertex& v) -> bool { return v.p.x >= -1 && v.p.x <= 1 && v.p.y >= -1 && v.p.y <= 1 && v.p.z >= 0 && v.p.z <= 1; };
			if (Inside(va) && Inside(vb) && Inside(vc) && TrianglesNormal(va.p, vb.p, vc.p).z < 0)
			{
				tri_new.push_back(a); tri_new.push_back(b); tri_new.push_back(c);
			}
		}
		triangles.swap(tri_new);
		tri_new.clear();

		// Mapping to screen
		std::vector<Point2I> screen_pos;
		for (auto& v : vertices)
		{
			screen_pos.push_back(Point2I(static_cast<int>((v.p.x + 1) * (width - 1) / 2), static_cast<int>((v.p.y + 1) * (height - 1) / 2)));
		}

		// Traverse all triangles and compute color for all sampling points (pixel shader)
		int size = height * width;
		float* zbuffer = new float[size];
		std::fill(zbuffer, zbuffer + size, 2.0f);
		std::fill(buffer, buffer + size, 0U);
		for (size_t i = 0; i < triangles.size(); i += 3)
		{
			int a = triangles[i], b = triangles[i + 1], c = triangles[i + 2];
			Vertex& va = vertices[a], vb = vertices[b], vc = vertices[c];
			Point2I pa = screen_pos[a], pb = screen_pos[b], pc = screen_pos[c];
			// Use z-buffer merge multiple colors
			DrawerZ drawer(buffer, width, height, zbuffer);
			drawer.Line(pa, pb, Drawer::Color(255, 255, 255), va.p.z, vb.p.z);
			drawer.Line(pa, pc, Drawer::Color(255, 255, 255), va.p.z, vc.p.z);
			drawer.Line(pb, pc, Drawer::Color(255, 255, 255), vb.p.z, vc.p.z);
		}
		delete[] zbuffer;

		return buffer;
	}
	std::shared_ptr<Mesh> CreateCubeMesh()
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		vertices.push_back(Point(0, 0, 0, 1));
		vertices.push_back(Point(0, 0, 1, 1));
		vertices.push_back(Point(1, 0, 0, 1));
		vertices.push_back(Point(1, 0, 1, 1));
		vertices.push_back(Point(0, 1, 0, 1));
		vertices.push_back(Point(0, 1, 1, 1));
		vertices.push_back(Point(1, 1, 0, 1));
		vertices.push_back(Point(1, 1, 1, 1));

		triangles.push_back(0); triangles.push_back(1); triangles.push_back(2);
		triangles.push_back(2); triangles.push_back(1); triangles.push_back(3);
		triangles.push_back(0); triangles.push_back(4); triangles.push_back(1);
		triangles.push_back(1); triangles.push_back(4); triangles.push_back(5);
		triangles.push_back(0); triangles.push_back(2); triangles.push_back(4);
		triangles.push_back(4); triangles.push_back(2); triangles.push_back(6);
		triangles.push_back(7); triangles.push_back(5); triangles.push_back(6);
		triangles.push_back(6); triangles.push_back(5); triangles.push_back(4);
		triangles.push_back(7); triangles.push_back(6); triangles.push_back(3);
		triangles.push_back(3); triangles.push_back(6); triangles.push_back(2);
		triangles.push_back(7); triangles.push_back(3); triangles.push_back(5);
		triangles.push_back(5); triangles.push_back(3); triangles.push_back(1);

		return std::make_shared<Mesh>(vertices, triangles);
	}
	std::shared_ptr<Mesh> SmoothSphereMesh(std::shared_ptr<Mesh> old)
	{
		auto& v_old = old->GetVertices();
		auto& tri_old = old->GetTriangles();
		size_t tri_n = old->TriangleCount();

		std::vector<Vertex> vertices(v_old);
		std::vector<int> triangles;

		for (size_t i = 0; i < tri_n; i++)
		{
			// get three vertex index
			int a = tri_old[3 * i], b = tri_old[3 * i + 1], c = tri_old[3 * i + 2];
			// compute center vertex on unit sphere
			Point center = (v_old[a].p + v_old[b].p + v_old[c].p) / 3;
			float length = VectorLength(center);
			if (length != 0)
				center /= length;
			center.w = 1;
			// add new vertex
			int d = static_cast<int>(vertices.size());
			vertices.push_back(center);
			// add new triangles
			triangles.push_back(a); triangles.push_back(b); triangles.push_back(d);
			triangles.push_back(b); triangles.push_back(c); triangles.push_back(d);
			triangles.push_back(c); triangles.push_back(a); triangles.push_back(d);
		}

		return std::make_shared<Mesh>(vertices, triangles);
	}
	std::shared_ptr<Mesh> CreateSphereMesh(int smooth)
	{
		std::vector<Vertex> vertices;
		std::vector<int> triangles;

		vertices.push_back(Point(0, 0, 1, 1));
		vertices.push_back(Point(1, 0, 0, 1));
		vertices.push_back(Point(0, 1, 0, 1));
		vertices.push_back(Point(-1, 0, 0, 1));
		vertices.push_back(Point(0, -1, 0, 1));
		vertices.push_back(Point(0, 0, -1, 1));

		triangles.push_back(0); triangles.push_back(2); triangles.push_back(1);
		triangles.push_back(0); triangles.push_back(3); triangles.push_back(2);
		triangles.push_back(0); triangles.push_back(4); triangles.push_back(3);
		triangles.push_back(0); triangles.push_back(1); triangles.push_back(4);
		triangles.push_back(5); triangles.push_back(1); triangles.push_back(2);
		triangles.push_back(5); triangles.push_back(2); triangles.push_back(3);
		triangles.push_back(5); triangles.push_back(3); triangles.push_back(4);
		triangles.push_back(5); triangles.push_back(4); triangles.push_back(1);

		auto pmesh = std::make_shared<Mesh>(vertices, triangles);

		for (; smooth > 0; smooth--)
			pmesh = SmoothSphereMesh(pmesh);

		return pmesh;
	}
	void AddObject(std::shared_ptr<Object> pobj)
	{
		objs.push_back(pobj);
	}
	bool RemoveObject(std::shared_ptr<Object> pobj)
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
	std::shared_ptr<Object> GetObject(std::shared_ptr<Object> prev)
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
	Drawer::Drawer(uint* _buffer, int _width, int _height)
		: buffer(_buffer), w(_width), h(_height)
	{
	}
	Drawer::~Drawer()
	{
	}
	uint Drawer::Color(uchar r, uchar g, uchar b)
	{
		return (static_cast<uint>(r) << 16) | (static_cast<uint>(g) << 8) | (static_cast<uint>(b) << 0);
	}
	void Drawer::Pixel(Point2I p, uint color)
	{
		//if (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h)
		int i = p.y * w + p.x;
		buffer[i] = color;
	}
	void Drawer::Line(Point2I p1, Point2I p2, uint color)
	{
		if (p1.x == p2.x && p1.y == p2.y)
			Pixel(p1, color);
		else if (p1.y == p2.y)
		{
			if (p2.x < p1.x)
				Swap(p1, p2);
			for (int x = p1.x; x <= p2.x; x++)
				Pixel(Point2I(x, p1.y), color);
		}
		else if (p1.x == p2.x)
		{
			if (p2.y < p1.y)
				Swap(p1, p2);
			for (int y = p1.y; y <= p2.y; y++)
				Pixel(Point2I(p1.x, y), color);
		}
		else
		{
			int dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					Swap(p1, p2);
				int y_move = (p2.y >= p1.y) ? 1 : -1;
				int y_offset = dx / 2; // dx times of 1/2
				for (auto p = p1; p.x <= p2.x; p.x++)
				{
					Pixel(p, color);
					y_offset += dy;     // dx times of dy/dx
					if (y_offset >= dx) // dx times of >= 1
					{
						y_offset -= dx; // dx times of -= 1
						p.y += y_move;
					}
				}
			}
			else
			{
				if (p2.y < p1.y)
					Swap(p1, p2);
				int x_move = (p2.x >= p1.x) ? 1 : -1;
				int x_offset = dy / 2;
				for (auto p = p1; p.y <= p2.y; p.y++)
				{
					Pixel(p, color);
					x_offset += dx;
					if (x_offset >= dy)
					{
						x_offset -= dy;
						p.x += x_move;
					}
				}
			}
		}
	}
	DrawerZ::DrawerZ(uint* _buffer, int _width, int _height, float* _zbuffer)
		: buffer(_buffer), w(_width), h(_height), zbuffer(_zbuffer)
	{
	}
	DrawerZ::~DrawerZ()
	{
	}
	uint DrawerZ::Color(uchar r, uchar g, uchar b)
	{
		return Drawer::Color(r, g, b);
	}
	void DrawerZ::Pixel(Point2I p, uint color, float z)
	{
		//if (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h && z >= 0 && z <= 1)
		int i = p.y * w + p.x;
		if (z < zbuffer[i])
		{
			buffer[i] = color;
			zbuffer[i] = z;
		}
	}
	void DrawerZ::Line(Point2I p1, Point2I p2, uint color, float z1, float z2)
	{
		if (p1.x == p2.x && p1.y == p2.y)
			Pixel(p1, color, Min(z1, z2));
		else if (p1.y == p2.y)
		{
			if (p2.x < p1.x)
				Swap(p1, p2), Swap(z1, z2);
			for (int x = p1.x; x <= p2.x; x++)
			{
				float t = static_cast<float>(x - p1.x) / (p2.x - p1.x);
				Pixel(Point2I(x, p1.y), color, Lerp(z1, z2, t));
			}
		}
		else if (p1.x == p2.x)
		{
			if (p2.y < p1.y)
				Swap(p1, p2), Swap(z1, z2);
			for (int y = p1.y; y <= p2.y; y++)
			{
				float t = static_cast<float>(y - p1.y) / (p2.y - p1.y);
				Pixel(Point2I(p1.x, y), color, Lerp(z1, z2, t));
			}
		}
		else
		{
			int dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					Swap(p1, p2), Swap(z1, z2);
				for (auto p = p1; p.x <= p2.x; p.x++)
				{
					float t = static_cast<float>(p.x - p1.x) / (p2.x - p1.x);
					p.y = Lerp(p1.y, p2.y, t);
					Pixel(p, color, Lerp(z1, z2, t));
				}
			}
			else
			{
				if (p2.y < p1.y)
					Swap(p1, p2), Swap(z1, z2);
				for (auto p = p1; p.y <= p2.y; p.y++)
				{
					float t = static_cast<float>(p.y - p1.y) / (p2.y - p1.y);
					p.x = Lerp(p1.x, p2.x, t);
					Pixel(p, color, Lerp(z1, z2, t));
				}
			}
		}
	}
}
