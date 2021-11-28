#define WIN32_LEAN_AND_MEAN
#include <iostream>
using std::cout;
using std::endl;
#include "rehenz.h"
using namespace Rehenz;
#include <string>
#include <timeapi.h>

int noise_example()
{
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

	return 0;
}

int render_soft_example()
{
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
	drawer.Line(Point2I(30, 30), Point2I(400, 200), drawer.Color(255U, 255U, 255U));
	drawer.Line(Point2I(50, 30), Point2I(420, 200), drawer.Color(255U, 0U, 0U));
	drawer.Line(Point2I(70, 30), Point2I(440, 200), drawer.Color(0U, 255U, 0U));
	drawer.Line(Point2I(90, 30), Point2I(460, 200), drawer.Color(0U, 0U, 255U));
	drawer.Line(Point2I(110, 30), Point2I(480, 200), drawer.Color(0U, 255U, 255U));
	drawer.Line(Point2I(130, 30), Point2I(500, 200), drawer.Color(255U, 0U, 255U));
	drawer.Line(Point2I(150, 30), Point2I(520, 200), drawer.Color(255U, 255U, 0U));
	srf_dx8.FillFromImage(buffer.get());

	cout << "Ready render" << endl;
	auto cube = CreateCubeMesh(std::vector<Vector>{
		Vector(1, 1, 1, 1), Vector(1, 0, 0, 1), Vector(0, 1, 0, 1), Vector(0, 0, 1, 1),
			Vector(1, 1, 0, 1), Vector(1, 0, 1, 1), Vector(0, 1, 1, 1), Vector(1, 1, 1, 1)});
	cout << "cube    vertex count: " << cube->VertexCount() << "  \ttriangle count: " << cube->TriangleCount() << endl;
	auto sphere = CreateSphereMesh();
	cout << "sphere  vertex count: " << sphere->VertexCount() << "  \ttriangle count: " << sphere->TriangleCount() << endl;
	auto sphereB = CreateSphereMeshB();
	cout << "sphereB vertex count: " << sphereB->VertexCount() << "  \ttriangle count: " << sphereB->TriangleCount() << endl;
	auto sphereC = CreateSphereMeshC();
	cout << "sphereC vertex count: " << sphereC->VertexCount() << "  \ttriangle count: " << sphereC->TriangleCount() << endl;
	auto sphereD = CreateSphereMeshD();
	cout << "sphereD vertex count: " << sphereD->VertexCount() << "  \ttriangle count: " << sphereD->TriangleCount() << endl;
	auto texture1 = CreateTexture1();
	auto textureC = CreateTextureC();
	PixelShader my_pixel_shader = [](const PixelShaderData& data, const Vertex& v0)->Vector
	{
		if (data.pobj->texture2 != nullptr)
			return data.pobj->texture2->GetColor(v0.uv2);
		else
			return v0.c;
	};
	Objects test1;
	for (int x = -10; x <= 10; x += 1)
	{
		auto obj_cube = std::make_shared<Object>(cube, texture1, textureC);
		obj_cube->position = Vector(static_cast<float>(x), -2, 0);
		obj_cube->scale = Vector(0.6f, 0.6f, 0.6f);
		test1.AddObject(obj_cube);
	}
	auto obj_sphere = std::make_shared<Object>(sphere);
	obj_sphere->position = Vector(0, 2, 0);
	obj_sphere->scale = Vector(0.4f, 0.4f, 0.4f);
	test1.AddObject(obj_sphere);
	auto obj_sphere2 = std::make_shared<Object>(sphereB);
	obj_sphere2->position = Vector(-2, 2, 0);
	obj_sphere2->scale = Vector(0.4f, 0.4f, 0.4f);
	test1.AddObject(obj_sphere2);
	auto obj_sphere3 = std::make_shared<Object>(sphereC);
	obj_sphere3->position = Vector(2, 2, 0);
	obj_sphere3->scale = Vector(0.4f, 0.4f, 0.4f);
	test1.AddObject(obj_sphere3);
	auto obj_sphere4 = std::make_shared<Object>(sphereD);
	obj_sphere4->position = Vector(0, 0, 0);
	obj_sphere4->scale = Vector(1, 1, 1);
	test1.AddObject(obj_sphere4);
	Objects test2;
	auto obj_cube = std::make_shared<Object>(cube, texture1, textureC);
	obj_cube->position = Vector(0, 0, 0);
	obj_cube->rotation = EulerAngles(0, -pi / 4, pi / 4 + pi / 24);
	obj_cube->scale = Vector(2.2f, 2.2f, 2.2f);
	test2.AddObject(obj_cube);
	Camera camera(height, width);
	Objects* scene = &test2;
	cout << "hold Enter to render" << endl;
	cout << "press 1/2/3/4/5 to switch render mode" << endl;
	cout << "press W/A/S/D to move camera" << endl;
	cout << "press I/J/K/L to rotate cube" << endl;
	cout << "press 8/9 to switch scene" << endl;

	cout << "Start fps counter" << endl;
	int fps[2] = { 0,0 };
	auto fps_t0 = timeGetTime();
	cout << "Lock fps" << endl;
	auto t0 = timeGetTime();
	cout << "press Q to exit" << endl;
	while (srf_dx8.GetWindowState())
	{
		// render
		if (KeyIsDown('1'))      camera.render_mode = Camera::RenderMode::Wireframe;
		else if (KeyIsDown('2')) camera.render_mode = Camera::RenderMode::PureWhite;
		else if (KeyIsDown('3')) camera.render_mode = Camera::RenderMode::Color;
		else if (KeyIsDown('4')) camera.render_mode = Camera::RenderMode::Texture;
		else if (KeyIsDown('5')) camera.render_mode = Camera::RenderMode::Shader;
		if (KeyIsDown('W'))      camera.position.y += 0.1f;
		else if (KeyIsDown('S')) camera.position.y -= 0.1f;
		if (KeyIsDown('A'))      camera.position.x -= 0.1f;
		else if (KeyIsDown('D')) camera.position.x += 0.1f;
		camera.at = -camera.position;
		if (KeyIsDown('I'))      obj_cube->rotation.theta += 0.05f;
		else if (KeyIsDown('K')) obj_cube->rotation.theta -= 0.05f;
		if (KeyIsDown('J'))      obj_cube->rotation.psi += 0.05f;
		else if (KeyIsDown('L')) obj_cube->rotation.psi -= 0.05f;
		if (KeyIsDown('8'))      scene = &test2;
		else if (KeyIsDown('9')) scene = &test1;
		if (KeyIsDown(VK_RETURN))
			srf_dx8.FillFromImage(camera.RenderImage(*scene, camera.DefaultVertexShader, my_pixel_shader));
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

int main()
{
	cout << "Hello~ Rehenz~" << endl;

	return render_soft_example();
}