#define WIN32_LEAN_AND_MEAN
#include <iostream>
using std::cout;
using std::endl;
#include "rehenz.h"
using namespace Rehenz;
#include <string>
#include <timeapi.h>

int main()
{
	cout << "Hello~ Rehenz~" << endl;

	float noise[50];
	Perlin1D(123U, 0, 10, 5, noise);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
			cout << noise[i * 5 + j] << " ";
		cout << endl;
	}

	cout << endl;
	PerlinNoise1D noise1(123);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
			cout << noise1.GetNoise(i + j / 5.0f) << " ";
		cout << endl;
	}

	cout << endl << "2-dim perlin and 3-dim perlin" << endl;
	PerlinNoise2D noise2(234);
	cout << noise2.GetNoise(1.1f, 2.3f) << endl; // -0.145062
	PerlinNoise3D noise3(345);
	cout << noise3.GetNoise(1.1f, 2.3f, 4.5f) << endl; // 0.0540515
	cout << noise2.GetNoiseSum(1.1f, 2.3f) << endl;    // -0.0163853
	cout << noise2.GetNoiseSumAbs(1.1f, 2.3f) << endl; // 0.258147
	cout << noise2.GetNoiseSumAbsSin(1.1f, 2.3f) << endl; // 0.999569



	cout << endl << "Open a surface with dx8" << endl;
	SurfaceDx8 srf_dx8;
	String title = "surface by dx8";
	int width = 800;
	int height = 600;
	srf_dx8.Create(GetModuleHandle(nullptr), width, height, title.c_str());
	cout << "Draw lines" << endl;
	int size = width * height;
	auto buffer = std::make_unique<uint[]>(size);
	auto drawer = Drawer(buffer.get(), width, height);
	drawer.Line(Point2I(30, 30), Point2I(400, 200), drawer.Color(255, 255, 255));
	drawer.Line(Point2I(50, 30), Point2I(420, 200), drawer.Color(255, 0, 0));
	drawer.Line(Point2I(70, 30), Point2I(440, 200), drawer.Color(0, 255, 0));
	drawer.Line(Point2I(90, 30), Point2I(460, 200), drawer.Color(0, 0, 255));
	drawer.Line(Point2I(110, 30), Point2I(480, 200), drawer.Color(0, 255, 255));
	drawer.Line(Point2I(130, 30), Point2I(500, 200), drawer.Color(255, 0, 255));
	drawer.Line(Point2I(150, 30), Point2I(520, 200), drawer.Color(255, 255, 0));
	srf_dx8.FillFromImage(buffer.get());

	cout << "Ready render" << endl;
	auto cube = CreateCubeMesh();
	cout << "cube    vertex count: " << cube->VertexCount() << "  \ttriangle count: " << cube->TriangleCount() << endl;
	auto sphere = CreateSphereMesh();
	cout << "sphere  vertex count: " << sphere->VertexCount() << "  \ttriangle count: " << sphere->TriangleCount() << endl;
	auto sphereB = CreateSphereMeshB();
	cout << "sphereB vertex count: " << sphereB->VertexCount() << "  \ttriangle count: " << sphereB->TriangleCount() << endl;
	//auto obj_cube = std::make_shared<Object>(cube);
	//AddObject(obj_cube);
	for (int x = -10; x <= 10; x += 1)
	{
		auto obj_cube = std::make_shared<Object>(cube);
		obj_cube->position = Vector(static_cast<float>(x), -2, 0);
		obj_cube->scale = Vector(0.4f, 0.4f, 0.4f);
		AddObject(obj_cube);
	}
	auto obj_sphere = std::make_shared<Object>(sphere);
	obj_sphere->position = Vector(-3, 3, 0);
	AddObject(obj_sphere);
	auto obj_sphere2 = std::make_shared<Object>(sphereB);
	obj_sphere2->position = Vector(0, 0, 0);
	AddObject(obj_sphere2);
	Camera camera(height, width);

	cout << "Start fps counter" << endl;
	int fps[2] = { 0,0 };
	auto fps_t0 = timeGetTime();
	cout << "Lock fps" << endl;
	auto t0 = timeGetTime();
	while (srf_dx8.GetWindowState())
	{
		// render
		if (KeyIsDown('W'))		 camera.position.y += 0.1f;
		else if (KeyIsDown('S')) camera.position.y -= 0.1f;
		if (KeyIsDown('A'))		 camera.position.x -= 0.1f;
		else if (KeyIsDown('D')) camera.position.x += 0.1f;
		camera.at = -camera.position;
		if (KeyIsDown('I'))		 obj_sphere2->rotation.theta += 0.05f;
		else if (KeyIsDown('K')) obj_sphere2->rotation.theta -= 0.05f;
		if (KeyIsDown('J'))		 obj_sphere2->rotation.psi += 0.05f;
		else if (KeyIsDown('L')) obj_sphere2->rotation.psi -= 0.05f;
		if (KeyIsDown(VK_RETURN))
			srf_dx8.FillFromImage(camera.RenderImage());
		// refresh
		srf_dx8.Present();
		// compute fps and set title
		fps[1]++;
		auto fps_t1 = timeGetTime();
		if (fps_t1 - fps_t0 >= 500)
		{
			srf_dx8.SetTitle((title + " fps: " + std::to_string(fps[0] + fps[1])).c_str());
			fps[0] = fps[1];
			fps[1] = 0;
			fps_t0 = fps_t1;
		}
		// sleep
		while (16 >= (timeGetTime() - t0))
			;
		t0 = timeGetTime();
		// exit
		if (KeyIsDown('Q'))
			break;
	}
	srf_dx8.Destroy();

	return 0;
}