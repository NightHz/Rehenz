#define WIN32_LEAN_AND_MEAN
#include <iostream>
using std::wcout;
using std::cout;
using std::endl;
#include "rehenz.h"
using namespace Rehenz;


int main_noise_example()
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

int main_surface_dx8_and_render_soft_example()
{
	cout << endl << "Open a surface with dx8" << endl;
	SurfaceDx8 srf_dx8;
	String title = "surface by dx8";
	int width = 800;
	int height = 600;
	srf_dx8.Create(GetModuleHandle(nullptr), width, height, title);

	cout << "Draw lines" << endl;
	int size = width * height;
	auto buffer = std::make_unique<uint[]>(size);
	auto drawer = Drawer(buffer.get(), width, height);
	drawer.Line(Point2I(30, 30), Point2I(400, 200), drawer.ColorRGB(255, 255, 255));
	drawer.Line(Point2I(50, 30), Point2I(420, 200), drawer.ColorRGB(255, 0, 0));
	drawer.Line(Point2I(70, 30), Point2I(440, 200), drawer.ColorRGB(0, 255U, 0));
	drawer.Line(Point2I(90, 30), Point2I(460, 200), drawer.ColorRGB(0, 0, 255));
	drawer.Line(Point2I(110, 30), Point2I(480, 200), drawer.ColorRGB(0, 255, 255));
	drawer.Line(Point2I(130, 30), Point2I(500, 200), drawer.ColorRGB(255, 0, 255));
	drawer.Line(Point2I(150, 30), Point2I(520, 200), drawer.ColorRGB(255, 255, 0));
	srf_dx8.FillFromImage(buffer.get());

	cout << "Ready render" << endl;
	// mesh
	auto cube = CreateCubeMeshColorful();
	cout << "cube    vertex count: " << cube->VertexCount() << "  \ttriangle count: " << cube->TriangleCount() << endl;
	auto sphere = CreateSphereMesh();
	cout << "sphere  vertex count: " << sphere->VertexCount() << "  \ttriangle count: " << sphere->TriangleCount() << endl;
	auto sphereB = CreateSphereMeshB();
	cout << "sphereB vertex count: " << sphereB->VertexCount() << "  \ttriangle count: " << sphereB->TriangleCount() << endl;
	auto sphereC = CreateSphereMeshC();
	cout << "sphereC vertex count: " << sphereC->VertexCount() << "  \ttriangle count: " << sphereC->TriangleCount() << endl;
	auto sphereD = CreateSphereMeshD();
	cout << "sphereD vertex count: " << sphereD->VertexCount() << "  \ttriangle count: " << sphereD->TriangleCount() << endl;
	auto frustum = CreateFrustumMesh(0);
	cout << "frustum vertex count: " << frustum->VertexCount() << "  \ttriangle count: " << frustum->TriangleCount() << endl;
	auto teapot = CreateMeshFromObjFile("assets/teapot.obj");
	cout << "teapot  vertex count: " << teapot->VertexCount() << "  \ttriangle count: " << teapot->TriangleCount() << endl;
	auto mug = CreateMeshFromObjFile("assets/mug.obj");
	cout << "mug     vertex count: " << mug->VertexCount() << "  \ttriangle count: " << mug->TriangleCount() << endl;
	// texture
	auto texture1 = CreateTexture1();
	auto textureC = CreateTextureC();
	auto texture_plaid = CreateTexturePlaid();
	auto texture_dice = CreateTextureDice();
	// scene
	RenderScene test1;
	for (float z = -10; z <= 10; z += 1)
	{
		for (float x = -10; x <= 10; x += 1)
		{
			auto obj_cube = std::make_shared<RenderObject>(cube, texture_plaid, texture1);
			obj_cube->transform.pos = Vector(x, -2, z);
			obj_cube->transform.scale = Vector(0.2f, 0.2f, 0.2f);
			test1.AddRenderObject(obj_cube);
		}
	}
	auto obj_sphere = std::make_shared<RenderObject>(sphere);
	obj_sphere->transform.pos = Vector(0, 2, 0);
	obj_sphere->transform.scale = Vector(0.4f, 0.4f, 0.4f);
	obj_sphere->texture = texture_plaid;
	test1.AddRenderObject(obj_sphere);
	auto obj_sphere2 = std::make_shared<RenderObject>(sphereB);
	obj_sphere2->transform.pos = Vector(-2, 2, 0);
	//obj_sphere2->scale = Vector(0.4f, 0.4f, 0.4f);
	obj_sphere2->texture = texture_plaid;
	test1.AddRenderObject(obj_sphere2);
	auto obj_sphere3 = std::make_shared<RenderObject>(sphereC);
	obj_sphere3->transform.pos = Vector(2, 2, 0);
	//obj_sphere3->scale = Vector(0.4f, 0.4f, 0.4f);
	obj_sphere3->texture = texture_plaid;
	test1.AddRenderObject(obj_sphere3);
	auto obj_sphere4 = std::make_shared<RenderObject>(sphereD);
	obj_sphere4->transform.pos = Vector(0, 0, 0);
	obj_sphere4->transform.scale = Vector(1, 1, 1);
	obj_sphere4->texture = texture_plaid;
	test1.AddRenderObject(obj_sphere4);
	auto obj_frustum = std::make_shared<RenderObject>(frustum);
	obj_frustum->transform.pos = Vector(3, 0, 0);
	obj_frustum->texture = texture_plaid;
	test1.AddRenderObject(obj_frustum);
	auto obj_teapot = std::make_shared<RenderObject>(teapot);
	obj_teapot->transform.pos = Vector(-3, -1, 0);
	obj_teapot->texture = texture_plaid;
	test1.AddRenderObject(obj_teapot);
	auto obj_mug = std::make_shared<RenderObject>(mug);
	obj_mug->transform.pos = Vector(-5, 1, 0);
	obj_mug->transform.axes.yaw = -pi_div2;
	obj_mug->transform.scale = Vector(0.6f, 0.6f, 0.6f);
	obj_mug->texture = texture_plaid;
	test1.AddRenderObject(obj_mug);
	RenderScene test2;
	auto obj_cube = std::make_shared<RenderObject>(cube, texture_plaid, texture_dice);
	obj_cube->transform.pos = Vector(0, 0, 0);
	//obj_cube->transform.axes = AircraftAxes(0, -pi / 4, pi / 4 + pi / 24);
	obj_cube->transform.axes = AircraftAxes(-1.55f, 1.15f, 0);
	obj_cube->transform.scale = Vector(1.1f, 1.1f, 1.1f);
	test2.AddRenderObject(obj_cube);
	// camera
	Camera camera(height, width);
	camera.transform.pos = Vector(-0.56f, 2.30f, -2.81f);
	camera.transform.axes = AircraftAxes(0.70f, 0.21f, 0);
	RenderScene* scene = &test2;
	cout << "hold Enter to render" << endl;
	cout << "press 1/2/3/4/5/6/7                  to switch render mode" << endl;
	cout << "press W/A/S/D/Space/LShift           to move camera" << endl;
	cout << "hold mouse middle key and move mouse to rotate camera" << endl;
	cout << "press I/J/K/L                        to rotate cube" << endl;
	cout << "press T/F/G/H/R/Y                    to move cube" << endl;
	cout << "press 8/9                            to switch scene" << endl;
	cout << "press Z/X                            to switch projection" << endl;
	// input
	Mouse mouse;

	cout << "press Q to exit" << endl;
	while (srf_dx8.GetWindowState())
	{
		// update input
		mouse.Present();
		float dt = srf_dx8.GetWindow()->fps_counter.GetLastDeltatime() / 1000.0f;

		// render
		if (KeyIsDown('1'))      camera.render_mode = Camera::RenderMode::Wireframe,
			camera.vertex_shader = DefaultVertexShader, camera.pixel_shader = DefaultPixelShader;
		else if (KeyIsDown('2')) camera.render_mode = Camera::RenderMode::PureWhite,
			camera.vertex_shader = DefaultVertexShader, camera.pixel_shader = DefaultPixelShader;
		else if (KeyIsDown('3')) camera.render_mode = Camera::RenderMode::Shader,
			camera.vertex_shader = DefaultVertexShader, camera.pixel_shader = DefaultPixelShader;
		else if (KeyIsDown('4')) camera.render_mode = Camera::RenderMode::Shader,
			camera.vertex_shader = DefaultVertexShader, camera.pixel_shader = TexturePixelShader;
		else if (KeyIsDown('5')) camera.render_mode = Camera::RenderMode::Shader,
			camera.vertex_shader = DefaultVertexShader, camera.pixel_shader = ps_light;
		else if (KeyIsDown('6')) camera.render_mode = Camera::RenderMode::Shader,
			camera.vertex_shader = vs_light, camera.pixel_shader = ps_color;
		else if (KeyIsDown('7')) camera.render_mode = Camera::RenderMode::Shader,
			camera.vertex_shader = vs_light, camera.pixel_shader = ps_tex;

		float cam_move_dis = 5 * dt;
		float cam_rotate_angle = 0.3f * dt;
		if (KeyIsDown('W'))      camera.transform.pos += cam_move_dis * camera.transform.GetFrontInGround();
		else if (KeyIsDown('S')) camera.transform.pos -= cam_move_dis * camera.transform.GetFrontInGround();
		if (KeyIsDown('A'))      camera.transform.pos -= cam_move_dis * camera.transform.GetRightInGround();
		else if (KeyIsDown('D')) camera.transform.pos += cam_move_dis * camera.transform.GetRightInGround();
		if (KeyIsDown(VK_SPACE)) camera.transform.pos.y += cam_move_dis;
		else if (KeyIsDown(VK_LSHIFT)) camera.transform.pos.y -= cam_move_dis;
		if (KeyIsDown(VK_MBUTTON))
		{
			camera.transform.axes.pitch += cam_rotate_angle * mouse.GetMoveY();
			camera.transform.axes.yaw += cam_rotate_angle * mouse.GetMoveX();
			mouse.SetToPrev();
		}

		float obj_move_dis = 4 * dt;
		float obj_rotate_angle = 3 * dt;
		if (KeyIsDown('I'))      obj_cube->transform.axes.pitch += obj_rotate_angle;
		else if (KeyIsDown('K')) obj_cube->transform.axes.pitch -= obj_rotate_angle;
		if (KeyIsDown('J'))      obj_cube->transform.axes.yaw += obj_rotate_angle;
		else if (KeyIsDown('L')) obj_cube->transform.axes.yaw -= obj_rotate_angle;
		if (KeyIsDown('F'))      obj_cube->transform.pos.x -= obj_move_dis;
		else if (KeyIsDown('H')) obj_cube->transform.pos.x += obj_move_dis;
		if (KeyIsDown('T'))      obj_cube->transform.pos.y += obj_move_dis;
		else if (KeyIsDown('G')) obj_cube->transform.pos.y -= obj_move_dis;
		if (KeyIsDown('R'))      obj_cube->transform.pos.z -= obj_move_dis;
		else if (KeyIsDown('Y')) obj_cube->transform.pos.z += obj_move_dis;
		if (KeyIsDown('8'))      scene = &test2;
		else if (KeyIsDown('9')) scene = &test1;

		if (KeyIsDown('Z'))      camera.projection.parallel_projection = false;
		else if (KeyIsDown('X')) camera.projection.parallel_projection = true;

		if (KeyIsDown(VK_RETURN))
			srf_dx8.FillFromImage(camera.RenderImage(*scene));
		// refresh
		srf_dx8.Present();
		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}
	srf_dx8.Destroy();

	return 0;
}

int main_clip_test()
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

int main_tilemap_and_path_finding_and_fps_counter_example()
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
	std::string tilemap_string{
		"*                      #                "
		"                       #                "
		"########               #     #          "
		"                       #     #          "
		"                       #     #          "
		"                             #          "
		"                             #          "
		"                       #######          "
		"                       #                "
		"                       #                "
		"                       #                "
		"                       #                "
		"                       #                "
		"      ##################                "
		"      #                #                "
		"      #                #                "
		"      #                #   ########     "
		"      #                #                "
		"      #                                 "
		"      #                                 "
		"      #   *                             "
		"      #                                 "
		"      #                                 "
		"      #                                 "
		"      #                                 "
		"      #                                 "
		"      #                                 "
		"      #######################           "
		"                   #                    "
		"                   #                    "
		"                   #                    "
		"                   #                    "
		"                   #                    "
		"           #########                    "
		"                   #                    "
		"                   #                    "
		"                                        "
		"                                        "
		"                                        "
		"                                        "
	};
	for (int y = 0; y < 40; y++)
	{
		for (int x = 0; x < 40; x++)
			tilemap(x, y) = (tilemap_string[x + static_cast<size_t>(y) * 40] == '#' ? 8 : 0);// (x + y) % 12;
	}

	cout << "Finding path of (0,0) and (0,0)" << endl;
	TilesPF tilemap_PF(&tilemap, std::set<uint>{8});
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
	PathFinding path_finding(&tilemap_PF(0, 0), &tilemap_PF(10, 20), "AStar");
	path_finding.Start();
	tilemap(0, 0) = 4;
	tilemap(10, 20) = 4;
	cout << "press N or M to next (only when key up -> key down)" << endl;
	bool press_n = false, press_m = false;
	cout << "press C to next (when key down)" << endl;

	cout << "press Q to exit" << endl;
	while (srf_dx8.GetWindowState())
	{
		float dt = srf_dx8.GetWindow()->fps_counter.GetLastDeltatime2();
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
				cout << "Search " << path_finding.SearchCount() << " nodes and find path : " << endl;
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
			else
			{
				cout << "No path" << endl;
			}
		}
		// render
		if (KeyIsDown(VK_LEFT)) tilemap.render_center.x -= dt * 20;
		else if (KeyIsDown(VK_RIGHT)) tilemap.render_center.x += dt * 20;
		if (KeyIsDown(VK_UP)) tilemap.render_center.y -= dt * 20;
		else if (KeyIsDown(VK_DOWN)) tilemap.render_center.y += dt * 20;
		if (KeyIsDown(VK_ADD))
		{
			tilemap.tile_size *= std::powf(1.6f, dt);
			//tilemap.tile_size += dt * Vector2(5, 5);
			tilemap.tile_size.x = Min(tilemap.tile_size.x, 40.0f);
			tilemap.tile_size.y = Min(tilemap.tile_size.y, 40.0f);
		}
		else if (KeyIsDown(VK_SUBTRACT))
		{
			tilemap.tile_size *= std::powf(1.6f, -dt);
			//tilemap.tile_size -= dt * Vector2(5, 5);
			tilemap.tile_size.x = Max(tilemap.tile_size.x, 2.0f);
			tilemap.tile_size.y = Max(tilemap.tile_size.y, 2.0f);
		}
		srf_dx8.FillFromImage(tilemap.Render(image.get(), width, height));
		// refresh
		srf_dx8.Present();
		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}
	srf_dx8.Destroy();

	return 0;
}

int main_two_surface_test()
{
	cout << endl << "Open a surface with dx8" << endl;
	SurfaceDx8 srf_dx8;
	const String title = "surface by dx8";
	const int width = 800;
	const int height = 600;
	std::unique_ptr<uint[]> image = std::make_unique<uint[]>(width * height);
	srf_dx8.Create(GetModuleHandle(nullptr), width, height, title.c_str());

	cout << "Open second surface with dx8" << endl;
	SurfaceDx8 srf_dx8_2;
	const String title2 = "second surface by dx8";
	std::unique_ptr<uint[]> image2 = std::make_unique<uint[]>(width * height);
	srf_dx8_2.Create(GetModuleHandle(nullptr), width, height, title2.c_str());

	cout << "press Q to exit" << endl;
	while (srf_dx8.GetWindowState() || srf_dx8_2.GetWindowState())
	{
		// refresh
		srf_dx8.Present();
		srf_dx8_2.Present();
		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}
	srf_dx8.Destroy();
	srf_dx8_2.Destroy();
	return 0;
}

int main_two_window_test()
{
	cout << endl << "Create window" << endl;
	const std::string title = "first window";
	const int width = 800;
	const int height = 600;
	SimpleWindowWithFC window(GetModuleHandle(nullptr), width, height, title);
	const std::string title2 = "second window";
	SimpleWindowWithFC window2(GetModuleHandle(nullptr), width, height, title2);

	cout << "press Q to exit" << endl;
	while (window.CheckWindowState() || window2.CheckWindowState())
	{
		// refresh
		window.Present();
		window2.Present();
		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}

	return 0;
}

int main_drawer_test()
{
	cout << endl;
	SurfaceDx8 srf;
	const int w = 40;
	const int h = 30;
	const int s = 20;
	const int w2 = w * s;
	const int h2 = h * s;
	srf.Create(GetModuleHandle(nullptr), w2, h2, "Rehenz drawer test");

	std::unique_ptr<uint[]> image = std::make_unique<uint[]>(w * h);
	Drawer drawer(image.get(), w, h);
	DrawerF drawerf(image.get(), w, h);
	std::unique_ptr<uint[]> image2 = std::make_unique<uint[]>(w2 * h2);
	Drawer drawer2(image2.get(), w2, h2);
	DrawerF drawerf2(image2.get(), w2, h2);
	while (srf.GetWindowState())
	{
		// time
		static ullong t = 0;
		uint dt = srf.GetWindow()->fps_counter.GetLastDeltatime();
		t += dt;
		// moving line
		auto GetMovingY = [](ullong t, ullong cycle, float y) -> float
		{
			t %= 8 * cycle;
			if (t < cycle)
				return 0;
			else if (t < 4 * cycle)
				return Lerp(0.0f, y, (t - cycle) / static_cast<float>(3 * cycle));
			else if (t < 5 * cycle)
				return y;
			else
				return Lerp(y, 0.0f, (t - 5 * cycle) / static_cast<float>(3 * cycle));
		};

		// clear
		const auto color_clear = Drawer::ColorRGB(255, 255, 255);
		drawer.Fill(color_clear);

		// test drawer
		const auto line_color = Drawer::ColorRGB(255, 0, 0);
		std::vector<std::pair<Point2I, Point2I>> lines;
		lines.emplace_back(Point2I(3, 4), Point2I(13, 6));
		lines.emplace_back(Point2I(10, 23), Point2I(5, 9));

		lines.emplace_back(Point2I(30, static_cast<int>(6.5f + GetMovingY(t, 300, 3))),
			Point2I(33, static_cast<int>(6.5f + GetMovingY(t + 300, 300, 3))));
		for (const auto& p : lines)
			drawer.Line(p.first, p.second, line_color);

		const auto line_f_color = Drawer::ColorRGB(0, 255, 0);
		std::vector<std::pair<Point2, Point2>> lines_f;
		lines_f.emplace_back(Point2(12.5f, 9.2f), Point2(17.5f, 11.2f));
		lines_f.emplace_back(Point2(12.5f, 13.5f), Point2(17.5f, 15.5f));
		lines_f.emplace_back(Point2(12.5f, 17.8f), Point2(17.5f, 19.8f));

		lines_f.emplace_back(Point2(20.2f, 3.8f), Point2(22.8f, 6.2f));
		lines_f.emplace_back(Point2(20.2f, 7.8f), Point2(22.8f, 10.38f));
		lines_f.emplace_back(Point2(20.2f, 11.8f), Point2(22.8f, 14.42f));
		lines_f.emplace_back(Point2(20.2f, 15.8f), Point2(22.8f, 17.6f));
		lines_f.emplace_back(Point2(20.2f, 19.58f), Point2(22.8f, 22.2f));
		lines_f.emplace_back(Point2(20.2f, 24.62f), Point2(22.8f, 27.2f));

		lines_f.emplace_back(Point2(27.8f, 6.2f), Point2(25.2f, 3.8f));
		lines_f.emplace_back(Point2(27.8f, 10.38f), Point2(25.2f, 7.8f));
		lines_f.emplace_back(Point2(27.8f, 14.42f), Point2(25.2f, 11.8f));
		lines_f.emplace_back(Point2(27.8f, 17.6f), Point2(25.2f, 15.8f));
		lines_f.emplace_back(Point2(27.8f, 22.2f), Point2(25.2f, 19.58f));
		lines_f.emplace_back(Point2(27.8f, 27.2f), Point2(25.2f, 24.62f));

		lines_f.emplace_back(Point2(12.8f, 21.8f), Point2(13.2f, 23.8f));
		lines_f.emplace_back(Point2(15.8f, 21.8f), Point2(16.2f, 22.1f));
		lines_f.emplace_back(Point2(15.8f, 24.3f), Point2(16.2f, 24.6f));

		lines_f.emplace_back(Point2(30.8f, 3.3f), Point2(w, 0));
		lines_f.emplace_back(Point2(30.8f, 24.3f), Point2(w, 15.6f));
		lines_f.emplace_back(Point2(33.8f, 27.3f), Point2(w, h));
		lines_f.emplace_back(Point2(30.8f, 27.3f), Point2(31.4f, h));
		lines_f.emplace_back(Point2(24.8f, h), Point2(27.4f, h));
		lines_f.emplace_back(Point2(14, 28), Point2(18, 28));

		lines_f.emplace_back(Point2(35.5f, 6.5f + GetMovingY(t, 300, 3)),
			Point2(38.5f, 6.5f + GetMovingY(t + 300, 300, 3)));
		for (const auto& p : lines_f)
			drawerf.Line(p.first, p.second, line_f_color);

		// expand image
		const auto edge_color = Drawer::ColorRGB(123, 141, 66);
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				drawer2.Rectangle(Point2I(x * s, y * s), Point2I(x * s + s - 2, y * s + s - 2), image[x + static_cast<size_t>(y) * w]);
				drawer2.Rectangle(Point2I(x * s + s - 1, y * s), Point2I(x * s + s - 1, y * s + s - 1), edge_color);
				drawer2.Rectangle(Point2I(x * s, y * s + s - 1), Point2I(x * s + s - 1, y * s + s - 1), edge_color);
			}
		}
		// highlight
		const auto highlight_color = Drawer::ColorRGB(0, 0, 0);
		const Point2I o(s / 2, s / 2);
		for (const auto& p : lines)
			drawer2.Line(p.first * s + o, p.second * s + o, highlight_color);
		for (const auto& p : lines_f)
			drawerf2.Line((p.first * s), (p.second * s), highlight_color);

		// refresh
		srf.FillFromImage(image2.get());
		srf.Present();
		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}

	return 0;
}

int main_drawer_test_triangle()
{
	cout << endl;
	SurfaceDx8 srf;
	const int w = 40;
	const int h = 30;
	const int s = 20;
	const int w2 = w * s;
	const int h2 = h * s;
	srf.Create(GetModuleHandle(nullptr), w2, h2, "Rehenz drawer test triangle");

	std::unique_ptr<uint[]> image = std::make_unique<uint[]>(w * h);
	Drawer drawer(image.get(), w, h);
	std::unique_ptr<uint[]> image2 = std::make_unique<uint[]>(w2 * h2);
	Drawer drawer2(image2.get(), w2, h2);
	while (srf.GetWindowState())
	{
		// clear
		const auto color_clear = Drawer::ColorRGB(255, 255, 255);
		drawer.Fill(color_clear);

		// test drawer
		const auto color = Drawer::ColorRGB(255, 0, 0);
		const auto line_color = Drawer::ColorRGB(0, 255, 0);
		std::vector<Point2I> tri;
		auto tri_add = [&tri](int x1, int y1, int x2, int y2, int x3, int y3)
		{
			tri.emplace_back(x1, y1); tri.emplace_back(x2, y2); tri.emplace_back(x3, y3);
		};
		tri_add(3, 4, 5, 6, 2, 8);
		tri_add(2, 15, 5, 10, 6, 18);
		tri_add(12, 5, 15, 4, 13, 18);

		tri_add(18, 3, 22, 3, 21, 3);
		tri_add(18, 6, 22, 8, 21, 6);
		tri_add(18, 14, 22, 11, 21, 14);

		tri_add(31, 10, 32, 4, 25, 18);
		tri_add(4, 20, 32, 24, 10, 26);
		for (size_t i = 0; i < tri.size(); i += 3)
		{
			drawer.Line(tri[i], tri[i + 1], line_color);
			drawer.Line(tri[i + 1], tri[i + 2], line_color);
			drawer.Line(tri[i], tri[i + 2], line_color);
			drawer.Triangle(tri[i], tri[i + 1], tri[i + 2], color);
		}

		// expand image
		const auto edge_color = Drawer::ColorRGB(123, 141, 66);
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				drawer2.Rectangle(Point2I(x * s, y * s), Point2I(x * s + s - 2, y * s + s - 2), image[x + static_cast<size_t>(y) * w]);
				drawer2.Rectangle(Point2I(x * s + s - 1, y * s), Point2I(x * s + s - 1, y * s + s - 1), edge_color);
				drawer2.Rectangle(Point2I(x * s, y * s + s - 1), Point2I(x * s + s - 1, y * s + s - 1), edge_color);
			}
		}
		// highlight
		const auto highlight_color = Drawer::ColorRGB(0, 0, 0);
		const Point2I o(s / 2, s / 2);
		for (size_t i = 0; i < tri.size(); i += 3)
		{
			drawer2.Line(tri[i] * s + o, tri[i + 1] * s + o, highlight_color);
			drawer2.Line(tri[i + 1] * s + o, tri[i + 2] * s + o, highlight_color);
			drawer2.Line(tri[i] * s + o, tri[i + 2] * s + o, highlight_color);
		}

		// refresh
		srf.FillFromImage(image2.get());
		srf.Present();
		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}

	return 0;
}

int main_drawerf_test_triangle()
{
	cout << endl;
	const int w = 20;
	const int h = 15;
	const int s = 40;
	const int w2 = w * s;
	const int h2 = h * s;
	SurfaceDx8 srf1;
	srf1.Create(GetModuleHandle(nullptr), w2, h2, "Rehenz drawerf test triangle : normal draw order");
	srf1.GetWindow()->fps_counter.LockFps(0);

	std::unique_ptr<uint[]> image1 = std::make_unique<uint[]>(w * h);
	DrawerF drawer1(image1.get(), w, h);
	std::unique_ptr<uint[]> image1_srf = std::make_unique<uint[]>(w2 * h2);
	Drawer drawer1_srf(image1_srf.get(), w2, h2);
	DrawerF drawerf1_srf(image1_srf.get(), w2, h2);

	SurfaceDx8 srf2;
	srf2.Create(GetModuleHandle(nullptr), w2, h2, "Rehenz drawerf test triangle : reverse draw order");
	srf2.GetWindow()->fps_counter.LockFps(0);

	std::unique_ptr<uint[]> image2 = std::make_unique<uint[]>(w * h);
	DrawerF drawer2(image2.get(), w, h);
	std::unique_ptr<uint[]> image2_srf = std::make_unique<uint[]>(w2 * h2);
	Drawer drawer2_srf(image2_srf.get(), w2, h2);
	DrawerF drawerf2_srf(image2_srf.get(), w2, h2);

	SurfaceDx8 srf3;
	srf3.Create(GetModuleHandle(nullptr), w2, h2, "Rehenz drawerf test triangle");
	srf3.GetWindow()->fps_counter.LockFps(0);

	std::unique_ptr<uint[]> image3_srf = std::make_unique<uint[]>(w2 * h2);
	Drawer drawer3_srf(image3_srf.get(), w2, h2);

	uint red = Drawer::ColorRGB(242, 129, 128);
	uint yellow = Drawer::ColorRGB(223, 218, 129);
	uint blue = Drawer::ColorRGB(141, 219, 254);
	uint green = Drawer::ColorRGB(149, 224, 129);
	uint purple = Drawer::ColorRGB(141, 133, 253);
	uint pink = Drawer::ColorRGB(231, 162, 244);
	uint orange = Drawer::ColorRGB(255, 178, 125);
	std::vector<Point2> tri;
	std::vector<uint> color;
	auto tri_add = [&tri, &color](float x1, float y1, float x2, float y2, float x3, float y3, uint c)
	{
		tri.emplace_back(x1 + 0.5f, y1 + 0.5f);
		tri.emplace_back(x2 + 0.5f, y2 + 0.5f);
		tri.emplace_back(x3 + 0.5f, y3 + 0.5f);
		color.push_back(c);
	};
	tri_add(0, 0, 4, 0, 2, 2, purple);
	tri_add(10, 0, 4, 0, 6, 3, green);
	tri_add(10, 0, 16, 0, 10.2f, 4.2f, red);
	tri_add(0, 0, 0, 5, 2, 2, red);
	tri_add(6, 3, 4, 0, 2, 2, yellow);

	tri_add(6, 3, 10, 0, 9, 6, purple);
	tri_add(10.2f, 4.2f, 10, 0, 9, 6, yellow);
	tri_add(10.2f, 4.2f, 16, 0, 11.3f, 5.4f, blue);
	tri_add(16, 9, 16, 0, 11.3f, 5.4f, green);
	tri_add(2, 2, 0, 5, 3, 6, green);

	tri_add(2, 2, 6, 3, 3, 6, pink);
	tri_add(6, 3, 3, 6, 6, 6, blue);
	tri_add(6, 3, 9, 6, 6, 6, red);
	tri_add(10.2f, 4.2f, 9, 6, 9.9f, 5.7f, green);
	tri_add(10.2f, 4.2f, 11.3f, 5.4f, 9.9f, 5.7f, purple);

	tri_add(0, 5, 0, 10, 3, 6, yellow);
	tri_add(6, 9, 0, 10, 3, 6, red);
	tri_add(6, 9, 6, 6, 3, 6, purple);
	tri_add(6, 9, 6, 6, 9, 6, pink);
	tri_add(6, 9, 9.6f, 6.3f, 9, 6, blue);

	tri_add(9, 6, 9.6f, 6.3f, 9.9f, 5.7f, orange);
	tri_add(16, 9, 9.6f, 6.3f, 9.9f, 5.7f, pink);
	tri_add(16, 9, 11.3f, 5.4f, 9.9f, 5.7f, yellow);
	tri_add(6, 9, 9.6f, 6.3f, 15, 10, yellow);
	tri_add(16, 9, 9.6f, 6.3f, 15, 10, red);

	tri_add(0, 10, 15, 10, 6, 9, green);
	tri_add(15, 10, 16, 10, 16, 9, purple);

	while (srf1.GetWindowState() || srf2.GetWindowState() || srf3.GetWindowState())
	{
		// clear
		const auto color_clear = Drawer::ColorRGB(255, 255, 255);
		drawer1.Fill(color_clear);
		drawer2.Fill(color_clear);
		drawer3_srf.Fill(color_clear);

		// draw triangle
		for (size_t i = 0; i < tri.size(); i += 3)
			drawer1.Triangle(tri[i], tri[i + 1], tri[i + 2], color[i / 3]);
		for (llong i = tri.size() - 3; i >= 0; i -= 3)
			drawer2.Triangle(tri[i], tri[i + 1], tri[i + 2], color[i / 3]);

		// expand image
		const auto edge_color = Drawer::ColorRGB(123, 141, 66);
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				drawer1_srf.Rectangle(Point2I(x * s, y * s), Point2I(x * s + s - 2, y * s + s - 2), image1[x + static_cast<size_t>(y) * w]);
				drawer1_srf.Rectangle(Point2I(x * s + s - 1, y * s), Point2I(x * s + s - 1, y * s + s - 1), edge_color);
				drawer1_srf.Rectangle(Point2I(x * s, y * s + s - 1), Point2I(x * s + s - 1, y * s + s - 1), edge_color);
				drawer2_srf.Rectangle(Point2I(x * s, y * s), Point2I(x * s + s - 2, y * s + s - 2), image2[x + static_cast<size_t>(y) * w]);
				drawer2_srf.Rectangle(Point2I(x * s + s - 1, y * s), Point2I(x * s + s - 1, y * s + s - 1), edge_color);
				drawer2_srf.Rectangle(Point2I(x * s, y * s + s - 1), Point2I(x * s + s - 1, y * s + s - 1), edge_color);
			}
		}
		// highlight
		const auto highlight_color = Drawer::ColorRGB(0, 0, 0);
		for (size_t i = 0; i < tri.size(); i += 3)
		{
			drawerf1_srf.Line(tri[i] * s, tri[i + 1] * s, highlight_color);
			drawerf1_srf.Line(tri[i + 1] * s, tri[i + 2] * s, highlight_color);
			drawerf1_srf.Line(tri[i] * s, tri[i + 2] * s, highlight_color);
			drawerf2_srf.Line(tri[i] * s, tri[i + 1] * s, highlight_color);
			drawerf2_srf.Line(tri[i + 1] * s, tri[i + 2] * s, highlight_color);
			drawerf2_srf.Line(tri[i] * s, tri[i + 2] * s, highlight_color);
			drawer3_srf.Triangle(Point2I(tri[i] * s), Point2I(tri[i + 1] * s), Point2I(tri[i + 2] * s), color[i / 3]);
			drawer3_srf.Line(Point2I(tri[i] * s), Point2I(tri[i + 1] * s), highlight_color);
			drawer3_srf.Line(Point2I(tri[i + 1] * s), Point2I(tri[i + 2] * s), highlight_color);
			drawer3_srf.Line(Point2I(tri[i] * s), Point2I(tri[i + 2] * s), highlight_color);
		}

		// refresh
		srf1.FillFromImage(image1_srf.get());
		srf1.Present();
		srf2.FillFromImage(image2_srf.get());
		srf2.Present();
		srf3.FillFromImage(image3_srf.get());
		srf3.Present();
		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}

	return 0;
}

int main_image_reader_text()
{
	// read image
	uint width;
	uint height;
	auto image = LoadImageFile(L"assets/NightHz.jpg", width, height);
	if (image == nullptr)
	{
		cout << "file not exist" << endl;
		return 1;
	}

	SurfaceDx8 srf_dx8;
	const String title = "image reader";
	srf_dx8.Create(GetModuleHandle(nullptr), width, height, title.c_str());

	cout << "press Q to exit" << endl;
	while (srf_dx8.GetWindowState())
	{
		// copy
		srf_dx8.FillFromImage(image.get());
		// refresh
		srf_dx8.Present();
		// msg
		SimpleMessageProcess();
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

	//return main_noise_example();
	//return main_surface_dx8_and_render_soft_example();
	//return main_clip_test();
	//return main_tilemap_and_path_finding_and_fps_counter_example();
	//return main_two_surface_test();
	//return main_two_window_test();
	//return main_drawer_test();
	//return main_drawer_test_triangle();
	//return main_drawerf_test_triangle();
	return main_image_reader_text();
}