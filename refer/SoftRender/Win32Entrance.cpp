// Win32Entrance.cpp
//
// By NightHz
//
// Application Entrance and Controll
//

#include "CSurface.h"
#include "SoftRender.h"
#include <time.h>
#define KEYDOWN(code) ((GetAsyncKeyState(code) & 0x8000) ? 1 : 0)

bool exitState = false;
CSurface surface;
TCHAR defaultTitle[] = TEXT("SoftRender");
const int width = 800;
const int height = 600;
UINT image[width*height];

Model models[1];
int modelN = 1;
Camera camera;

void LoadModel(void);
void Application(void);
void CountFPS(void);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	if (!surface.Create(hinstance, width, height, defaultTitle))
		return 1;
	LoadModel();
	while (!exitState)
	{
		Application();
		SoftRender(models, modelN, camera, image, width, height);
		if (!surface.FillFromImage(image))
			return 1;
		if (!surface.Present(exitState))
			return 1;
		CountFPS();
	}
	surface.Destroy();
	return 0;
}

void CountFPS(void)
{
	static int fps = 0;
	static clock_t time = clock();
	fps++;
	if (clock() - time >= 1000)
	{
		TCHAR title[160];
		wsprintf(title, "%s \t fps:%d", defaultTitle, fps);
		surface.SetTitle(title);
		time = clock();
		fps = 0;
	}
}

void SetTriangle(Model model, int triIndex, int v1, int v2, int v3)
{
	int index = triIndex * 3;
	model.triangles[index] = v1;
	model.triangles[index + 1] = v2;
	model.triangles[index + 2] = v3;
}

void LoadModel(void)
{
	camera.position = Point(0, 0, 4.0f, 1.0f);
	camera.oriented = EulerAngles(0, 0, 0);

	models[0].position = Point(0, 0, 0, 1.0f);
	models[0].oriented = EulerAngles(0, 0, 0);
	models[0].vertices = new Vertex[8];
	models[0].vertexN = 8;
	models[0].vertices[0] = Vertex(Point(-1.0f, -1.0f, -1.0f, 1.0f), Color(0, 0, 0, 1.0f));
	models[0].vertices[1] = Vertex(Point(-1.0f, -1.0f,  1.0f, 1.0f), Color(0, 0, 1.0f, 1.0f));
	models[0].vertices[2] = Vertex(Point( 1.0f, -1.0f, -1.0f, 1.0f), Color(1.0f, 0, 0, 1.0f));
	models[0].vertices[3] = Vertex(Point( 1.0f, -1.0f,  1.0f, 1.0f), Color(1.0f, 0, 1.0f, 1.0f));
	models[0].vertices[4] = Vertex(Point(-1.0f,  1.0f, -1.0f, 1.0f), Color(0, 1.0f, 0, 1.0f));
	models[0].vertices[5] = Vertex(Point(-1.0f,  1.0f,  1.0f, 1.0f), Color(0, 1.0f, 1.0f, 1.0f));
	models[0].vertices[6] = Vertex(Point( 1.0f,  1.0f, -1.0f, 1.0f), Color(1.0f, 1.0f, 0, 1.0f));
	models[0].vertices[7] = Vertex(Point( 1.0f,  1.0f,  1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
	models[0].triangles = new int[36];
	models[0].triangleN = 12;
	SetTriangle(models[0], 0, 0, 1, 2);
	SetTriangle(models[0], 1, 2, 1, 3);
	SetTriangle(models[0], 2, 0, 4, 1);
	SetTriangle(models[0], 3, 1, 4, 5);
	SetTriangle(models[0], 4, 0, 2, 4);
	SetTriangle(models[0], 5, 4, 2, 6);
	SetTriangle(models[0], 6, 7, 5, 6);
	SetTriangle(models[0], 7, 6, 5, 4);
	SetTriangle(models[0], 8, 7, 6, 3);
	SetTriangle(models[0], 9, 3, 6, 2);
	SetTriangle(models[0], 10, 7, 3, 5);
	SetTriangle(models[0], 11, 5, 3, 1);
}

void Application(void)
{
	if (KEYDOWN(VK_ESCAPE))
		exitState = true;
}
