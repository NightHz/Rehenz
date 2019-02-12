// HSoftRender.cpp
//
// By NightHz
//

#include "HSoftRender.h"


UINT Rehenz::SoftRender::World::CloneObjectToWorld(Object * obj, Point position, EulerAngles oriented, Vector scale)
{
	Object * newObj = new Object();
	obj->Clone(newObj);

	for (unsigned int i = 0;i < size;i++)
	{
		if (objs[i] == nullptr)
		{
			objs[i] = newObj;
			return i;
		}
	}

	// 空间不足，扩大尺寸
	unsigned int newSize = size + cSize;
	Object** newObjs = new Object*[newSize];
	::memset(newObjs, 0, sizeof(newObjs));
	::memcpy_s(newObjs, sizeof(newObjs), objs, sizeof(objs));

	Object** clsObjs = objs;
	unsigned int clsSize = size;
	objs = newObjs;
	size = newSize;
	objs[clsSize] = newObj;

	// 清理
	for (unsigned int i = 0;i < clsSize;i++)
		if (clsObjs[i])
			delete clsObjs[i];
	delete[] clsObjs;

	return clsSize;
}

namespace Rehenz
{
	namespace SoftRender
	{

	}
}

void Rehenz::SoftRender::World::RenderImage(UINT * image, int width, int height)
{
	::memset(image, 0, width*height * sizeof(UINT));
}
