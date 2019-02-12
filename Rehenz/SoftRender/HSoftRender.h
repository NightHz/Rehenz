// HSoftRender.h
//
// By NightHz
//
//
//

#ifndef _HSOFTRENDER_H_
#define _HSOFTRENDER_H_

#include <memory.h>
#include "HRenderMath.h"

typedef unsigned int UINT;

namespace Rehenz
{
	namespace SoftRender
	{
		struct Vertex
		{
			Point position;
			UINT color;

			Vertex() {}
			Vertex(Point p, UINT c)
			{
				position = p;
				color = c;
			}
		};

		class Object
		{
		public:
			Point position;
			EulerAngles oriented;
			Vector scale;

			Vertex * vertices;
			int vertexN;
			int * triangles;
			int triangleN;

			Object()
			{
				vertices = nullptr;
				triangles = nullptr;
			}
			~Object()
			{
				if (vertices)
					delete[] vertices;
				if (triangles)
					delete[] triangles;
			}

			void Clone(Object* obj)
			{
				obj->position = position;
				obj->oriented = oriented;
				obj->scale = scale;
				obj->vertexN = vertexN;
				obj->vertices = new Vertex[vertexN];
				memcpy_s(obj->vertices, sizeof(obj->vertices), vertices, sizeof(vertices));
				obj->triangleN = triangleN;
				obj->triangles = new int[3 * triangleN];
				memcpy_s(obj->triangles, sizeof(obj->triangles), triangles, sizeof(triangles));
			}
		};

		struct Camera
		{
			Point position;
			EulerAngles oriented;
			float fovy;
			float near_z;
			float far_z;
		};

		enum RenderMode { LINE, COLOR, UV };

		class World
		{
		private:
			Object** objs;
			const unsigned int cSize;
			unsigned int size;

		public:
			Camera cam;
			RenderMode renderMode;

		private:

		public:
			// size为能够容纳的object数量
			World(unsigned int size) : cSize(size)
			{
				this->size = size;
				objs = new Object *[size];
				::memset(objs, 0, sizeof(objs));
				renderMode = LINE;
				cam.position = Vector(0, 0, 0, 1);
				cam.oriented = EulerAngles(0, 0, 0);
				cam.fovy = pi / 2;
				cam.near_z = 1.0f;
				cam.far_z = 500.0f;
			}
			~World()
			{
				ClearWorld();
				delete[] objs;
			}

			// return objectID
			UINT CloneObjectToWorld(Object* obj, Point position, EulerAngles oriented, Vector scale);

			void ClearObject(UINT objectID)
			{
				if (objs[objectID])
				{
					delete objs[objectID];
					objs[objectID] = nullptr;
				}
			}

			void ClearWorld()
			{
				for (unsigned int i = 0;i < size;i++)
					ClearObject(i);
			}

			unsigned int GetSize() { return size; }

			void RenderImage(UINT * image, int width, int height);
		};
	}
}

#endif // !_HSOFTRENDER_H_
