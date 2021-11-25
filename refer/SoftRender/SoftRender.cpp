// SoftRender.cpp
//
// By NightHz
//

#include "SoftRender.h"

Matrix mToWorld;
Matrix mToCamera;
Matrix mToProjection;
Matrix mToScreen;
Matrix mTransform;

Vertex * vertices;
int vertexN;
int * triangles;
int triangleN;

void SoftRender(Model * models, int modelN, Camera camera, UINT * image, int width, int height)
{
	vertexN = 0;
	triangleN = 0;
	for (int i = 0; i < modelN; i++)
	{
		vertexN += models[i].vertexN;
		triangleN += models[i].triangleN;
	}
	vertices = new Vertex[vertexN];
	triangles = new int[triangleN * 3];
	mToCamera = GetInverseMatrixT(camera.position) * GetInverseMatrixE(camera.oriented);
	mToProjection = GetMatrixP(pi / 2, (float)width / height, 1.0f, 500.0f);
	mToScreen = GetMatrixT(1.0f, 1.0f, 0) * GetMatrixS((float)width, (float)height, 1);

	GeometryProcessing(models, modelN);
	Rasterization();

	delete vertices;
	delete triangles;
}

void GeometryProcessing(Model * models, int modelN)
{
	Vertex * ver = vertices;
	int * tri = triangles;
	int vN = 0;
	for (int i = 0; i < modelN; i++)
	{
		mToWorld = GetMatrixE(models[i].oriented) * GetMatrixT(models[i].position);
		mTransform = mToWorld * mToCamera * mToProjection;
		for (int j = 0; j < models[i].triangleN * 3; j++)
			tri[j] = models[i].triangles[j] + vN;
		tri += models[i].triangleN * 3;
		for (int j = 0; j < models[i].vertexN; j++)
			ver[j] = VertexShader(models[i].vertices[j]);
		ver += models[i].vertexN;
		vN += models[i].vertexN;
	}

}

Vertex VertexShader(Vertex v)
{
	v.position = PointStandard(v.position * mTransform);
	return v;
}