// SoftRender.h
//
// By NightHz
//

#pragma once

#include "MathLib.h"

typedef unsigned int UINT;
typedef Quaternion Color;

struct Vertex
{
	Point position;
	Color color;

	Vertex() {}
	Vertex(Point p, Color c)
	{
		position = p;
		color = c;
	}
};

struct Model
{
	Point position;
	EulerAngles oriented;

	Vertex * vertices;
	int vertexN;
	int * triangles;
	int triangleN;
};

struct Camera
{
	Point position;
	EulerAngles oriented;
};

void SoftRender(Model * models, int modelN, Camera camera, UINT * image, int width, int height);
void GeometryProcessing(void);
void Rasterization(void);

Vertex VertexShader(Vertex v);
Color PixelShader(Point2D p);
