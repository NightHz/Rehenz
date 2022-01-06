#define WIN32_LEAN_AND_MEAN
#include <iostream>
using std::cout;
using std::endl;
#include "rehenz.h"
using namespace Rehenz;
#include <string>
#include <Windows.h>
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
	// mesh
	auto cube = CreateCubeMesh(std::vector<Color>{
		Color(1, 1, 1), Color(1, 0, 0), Color(0, 1, 0), Color(0, 0, 1),
			Color(1, 1, 0), Color(1, 0, 1), Color(0, 1, 1), Color(1, 1, 1)});
	cout << "cube    vertex count: " << cube->VertexCount() << "  \ttriangle count: " << cube->TriangleCount() << endl;
	auto sphere = CreateSphereMesh();
	cout << "sphere  vertex count: " << sphere->VertexCount() << "  \ttriangle count: " << sphere->TriangleCount() << endl;
	auto sphereB = CreateSphereMeshB();
	cout << "sphereB vertex count: " << sphereB->VertexCount() << "  \ttriangle count: " << sphereB->TriangleCount() << endl;
	auto sphereC = CreateSphereMeshC();
	cout << "sphereC vertex count: " << sphereC->VertexCount() << "  \ttriangle count: " << sphereC->TriangleCount() << endl;
	auto sphereD = CreateSphereMeshD();
	cout << "sphereD vertex count: " << sphereD->VertexCount() << "  \ttriangle count: " << sphereD->TriangleCount() << endl;
	// texture
	auto texture1 = CreateTexture1();
	auto textureC = CreateTextureC();
	auto texture_plaid = CreateTexturePlaid();
	auto texture_dice = CreateTextureDice();
	// shader
	PixelShader my_pixel_shader = [](const PixelShaderData& data, const Vertex& v0)->Color
	{
		//(data);
		//return Color(v0.uv.x, v0.uv.y, 0.8f);
		if (data.pobj->texture2 != nullptr)
			return data.pobj->texture2->GetColor(v0.uv2);
		else
			return Color(0.1f, 0.1f, 0.1f);
	};
	// scene
	RenderObjects test1;
	for (float z = -10; z <= 10; z += 1)
	{
		for (float x = -10; x <= 10; x += 1)
		{
			auto obj_cube = std::make_shared<RenderObject>(cube, texture_plaid, texture1);
			obj_cube->position = Vector(x, -2, z);
			obj_cube->scale = Vector(0.4f, 0.4f, 0.4f);
			test1.AddObject(obj_cube);
		}
	}
	auto obj_sphere = std::make_shared<RenderObject>(sphere);
	obj_sphere->position = Vector(0, 2, 0);
	obj_sphere->scale = Vector(0.4f, 0.4f, 0.4f);
	test1.AddObject(obj_sphere);
	auto obj_sphere2 = std::make_shared<RenderObject>(sphereB);
	obj_sphere2->position = Vector(-2, 2, 0);
	obj_sphere2->scale = Vector(0.4f, 0.4f, 0.4f);
	test1.AddObject(obj_sphere2);
	auto obj_sphere3 = std::make_shared<RenderObject>(sphereC);
	obj_sphere3->position = Vector(2, 2, 0);
	obj_sphere3->scale = Vector(0.4f, 0.4f, 0.4f);
	test1.AddObject(obj_sphere3);
	auto obj_sphere4 = std::make_shared<RenderObject>(sphereD);
	obj_sphere4->position = Vector(0, 0, 0);
	obj_sphere4->scale = Vector(1, 1, 1);
	test1.AddObject(obj_sphere4);
	RenderObjects test2;
	auto obj_cube = std::make_shared<RenderObject>(cube, texture_plaid, texture_dice);
	obj_cube->position = Vector(0, 0, 0);
	obj_cube->rotation = EulerAngles(0, -pi / 4, pi / 4 + pi / 24);
	obj_cube->scale = Vector(2.2f, 2.2f, 2.2f);
	test2.AddObject(obj_cube);
	// camera
	Camera camera(height, width);
	RenderObjects* scene = &test2;
	cout << "hold Enter to render" << endl;
	cout << "press 1/2/3/4/5   to switch render mode" << endl;
	cout << "press W/A/S/D     to move camera" << endl;
	cout << "press I/J/K/L     to rotate cube" << endl;
	cout << "press T/F/G/H/R/Y to move cube" << endl;
	cout << "press 8/9         to switch scene" << endl;

	cout << "Start fps counter" << endl;
	int fps[2] = { 0,0 };
	auto fps_t0 = timeGetTime();
	bool lock_fps = true;
	if (lock_fps)
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
		if (KeyIsDown('F'))      obj_cube->position.x -= 0.1f;
		else if (KeyIsDown('H')) obj_cube->position.x += 0.1f;
		if (KeyIsDown('T'))      obj_cube->position.y += 0.1f;
		else if (KeyIsDown('G')) obj_cube->position.y -= 0.1f;
		if (KeyIsDown('R'))      obj_cube->position.z -= 0.1f;
		else if (KeyIsDown('Y')) obj_cube->position.z += 0.1f;
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
		if (lock_fps)
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

int clip_example()
{
	cout << endl;

	Point2 p1 = Point2(-0.3f, 0.0f);
	Point2 p2 = Point2(1.0f, 2.0f);
	cout << "before: p1(" << p1.x << "," << p1.y << ") p2(" << p2.x << "," << p2.y << ")" << endl;
	cout << "clip... :" << ClipLine2DCohenSutherland(p1, p2, 1, 1) << endl;
	cout << "after : p1(" << p1.x << "," << p1.y << ") p2(" << p2.x << "," << p2.y << ")" << endl;

	Point3 p3 = Point3(-0.3f, -1.0f, -3.2f);
	Point3 p4 = Point3(1.0f, 2.0f, 5.0f);
	cout << "before: p3(" << p3.x << "," << p3.y << "," << p3.z << ") p4(" << p4.x << "," << p4.y << "," << p4.z << ")" << endl;
	cout << "clip... :" << ClipLine3DCohenSutherland(p3, p4, 1, 1, 1) << endl;
	cout << "after : p3(" << p3.x << "," << p3.y << "," << p3.z << ") p4(" << p4.x << "," << p4.y << "," << p4.z << ")" << endl;

	return 0;
}

int tilemap_and_path_finding_example()
{
	cout << endl << "Open a surface with dx8" << endl;
	SurfaceDx8 srf_dx8;
	String title = "surface by dx8";
	const int width = 800;
	const int height = 600;
	std::unique_ptr<uint[]> image = std::make_unique<uint[]>(width * height);
	srf_dx8.Create(GetModuleHandle(nullptr), width, height, title.c_str());

	cout << "Create a tilemap" << endl;
	Tilemap tilemap(40, 40);
	for (int y = 0; y < 40; y++)
	{
		for (int x = 0; x < 40; x++)
			tilemap(x, y) = (x + y) % 12;
	}

	cout << "Finding path of (0,0) and (0,0)" << endl;
	TilesPF tilemap_PF(&tilemap);
	cout << "Dijkstra Path : " << endl;
	auto path = PathFindingDijkstra(&tilemap_PF(0, 0), &tilemap_PF(0, 0));
	for (auto node : path)
	{
		auto tile = static_cast<TilePF*>(node);
		cout << "(" << tile->GetX() << "," << tile->GetY() << ")  \t";
	}
	cout << endl;
	cout << "A star Path : " << endl;
	auto path2 = PathFindingAStar(&tilemap_PF(0, 0), &tilemap_PF(0, 0));
	for (auto node : path2)
	{
		auto tile = static_cast<TilePF*>(node);
		cout << "(" << tile->GetX() << "," << tile->GetY() << ")  \t";
	}
	cout << endl;

	cout << "Start path finding coroutine" << endl;
	PathFinding path_finding(&tilemap_PF(0, 0), &tilemap_PF(10, 20), "A");
	path_finding.Start();
	cout << "press N or M to next" << endl;
	bool press_n = false, press_m = false;
	cout << "press C to next faster" << endl;

	cout << "Start fps counter" << endl;
	FpsCounter fps_counter;
	fps_counter.UpdateFpsCallback = [&srf_dx8, &title](uint fps)
	{
		srf_dx8.SetTitle((title + " fps: " + std::to_string(fps)).c_str());
	};

	cout << "press Q to exit" << endl;
	while (srf_dx8.GetWindowState())
	{
		// next
		bool next = false;
		if (!press_n && KeyIsDown('N'))
			press_n = true, next = true;
		else if (press_n && KeyIsUp('N'))
			press_n = false;
		if (!press_m && KeyIsDown('M'))
			press_m = true, next = true;
		else if (press_m && KeyIsUp('M'))
			press_m = false;
		if ((next || KeyIsDown('C')) && !path_finding.IsFinished())
		{
			cout << "run path finding once" << endl;
			path_finding.Next(1);
			// modify tilemap
			if (!path_finding.IsFinished())
			{
				for (int y = 0; y < 40; y++)
				{
					for (int x = 0; x < 40; x++)
					{
						const auto& tile_PF = tilemap_PF(x, y);
						if (tile_PF.IsClose())
							tilemap(x, y) = 3;
						else if (tile_PF.IsOpen())
							tilemap(x, y) = 2;
						else
							tilemap(x, y) = 0;
					}
				}
				auto min = path_finding.GetMinOpenNode();
				if (min != nullptr)
				{
					auto tile_PF = static_cast<TilePF*>(min);
					tilemap(tile_PF->GetX(), tile_PF->GetY()) = 1;
				}
				tilemap(0, 0) = 4;
				tilemap(10, 20) = 4;
			}
			// check finish
			else if (path_finding.IsFinished() && path_finding.ExistPath())
			{
				cout << "Find path : " << endl;
				auto path3 = path_finding.GetPath();
				for (auto node : path3)
				{
					auto tile_PF = static_cast<TilePF*>(node);
					cout << "(" << tile_PF->GetX() << "," << tile_PF->GetY() << ")  \t";
					tilemap(tile_PF->GetX(), tile_PF->GetY()) = 5;
				}
				cout << endl;
				tilemap(0, 0) = 4;
				tilemap(10, 20) = 4;
			}
		}
		// render
		srf_dx8.FillFromImage(tilemap.Render(image.get(), width, height));
		// refresh
		srf_dx8.Present();
		// update fps
		fps_counter.Present();
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

	//return noise_example();
	//return render_soft_example();
	//return clip_example();
	return tilemap_and_path_finding_example();
}