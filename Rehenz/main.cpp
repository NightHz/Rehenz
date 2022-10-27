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
			camera.vertex_shader = DefaultVertexShader, camera.pixel_shader = Shader::ps_light;
		else if (KeyIsDown('6')) camera.render_mode = Camera::RenderMode::Shader,
			camera.vertex_shader = Shader::vs_light, camera.pixel_shader = Shader::ps_color;
		else if (KeyIsDown('7')) camera.render_mode = Camera::RenderMode::Shader,
			camera.vertex_shader = Shader::vs_light, camera.pixel_shader = Shader::ps_tex;

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
	std::unique_ptr<uint[]> image = std::make_unique<uint[]>(static_cast<size_t>(width) * height);
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
	std::unique_ptr<uint[]> image = std::make_unique<uint[]>(static_cast<size_t>(width) * height);
	srf_dx8.Create(GetModuleHandle(nullptr), width, height, title.c_str());

	cout << "Open second surface with dx8" << endl;
	SurfaceDx8 srf_dx8_2;
	const String title2 = "second surface by dx8";
	std::unique_ptr<uint[]> image2 = std::make_unique<uint[]>(static_cast<size_t>(width) * height);
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

	std::unique_ptr<uint[]> image = std::make_unique<uint[]>(static_cast<size_t>(w) * h);
	Drawer drawer(image.get(), w, h);
	DrawerF drawerf(image.get(), w, h);
	std::unique_ptr<uint[]> image2 = std::make_unique<uint[]>(static_cast<size_t>(w2) * h2);
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

	std::unique_ptr<uint[]> image = std::make_unique<uint[]>(static_cast<size_t>(w) * h);
	Drawer drawer(image.get(), w, h);
	std::unique_ptr<uint[]> image2 = std::make_unique<uint[]>(static_cast<size_t>(w2) * h2);
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

	std::unique_ptr<uint[]> image1 = std::make_unique<uint[]>(static_cast<size_t>(w) * h);
	DrawerF drawer1(image1.get(), w, h);
	std::unique_ptr<uint[]> image1_srf = std::make_unique<uint[]>(static_cast<size_t>(w2) * h2);
	Drawer drawer1_srf(image1_srf.get(), w2, h2);
	DrawerF drawerf1_srf(image1_srf.get(), w2, h2);

	SurfaceDx8 srf2;
	srf2.Create(GetModuleHandle(nullptr), w2, h2, "Rehenz drawerf test triangle : reverse draw order");
	srf2.GetWindow()->fps_counter.LockFps(0);

	std::unique_ptr<uint[]> image2 = std::make_unique<uint[]>(static_cast<size_t>(w) * h);
	DrawerF drawer2(image2.get(), w, h);
	std::unique_ptr<uint[]> image2_srf = std::make_unique<uint[]>(static_cast<size_t>(w2) * h2);
	Drawer drawer2_srf(image2_srf.get(), w2, h2);
	DrawerF drawerf2_srf(image2_srf.get(), w2, h2);

	SurfaceDx8 srf3;
	srf3.Create(GetModuleHandle(nullptr), w2, h2, "Rehenz drawerf test triangle");
	srf3.GetWindow()->fps_counter.LockFps(0);

	std::unique_ptr<uint[]> image3_srf = std::make_unique<uint[]>(static_cast<size_t>(w2) * h2);
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

int main_image_reader_test()
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

int main_d3d12_example()
{
	cout << endl << "create window ..." << endl;
	const std::string title = "d3d12 example";
	const int width = 800;
	const int height = 600;
	auto window = std::make_unique<SimpleWindowWithFC>(GetModuleHandle(nullptr), width, height, title);
	if (!window->CheckWindowState())
		return 1;
	Mouse mouse;
	FpsCounterS fps_counter;
	fps_counter.LockFps(0);

	cout << "create d3d12 device ..." << endl;
	auto device = std::make_unique<D3d12Device>();
	auto SafeReturn = [&device](int return_v)
	{
		if (device)
			device->FlushGpu();
		return return_v;
	};
	auto cmd_list = device->Create(window.get());
	if (!cmd_list)
		return SafeReturn(1);

	cout << "setup resource ..." << endl;
	D3D12_RESOURCE_DESC rc_desc{};
	D3D12_RESOURCE_BARRIER rc_barr[4]{};
	std::shared_ptr<Mesh> mesh0;
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
	D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc{};

	// create target
	auto target = std::make_shared<D3d12RenderTarget>(width, height, device->GetScFormat(), true, true, Color::yellow_green_o, 0, 0, device.get());
	if (!*target)
		return SafeReturn(1);

	// create mesh
	mesh0 = CreateCubeMeshColorful();
	auto cube = std::make_shared<D3d12Mesh>(mesh0.get(), device.get(), cmd_list);
	if (!*cube)
		return SafeReturn(1);
	mesh0 = CreateMeshFromObjFile("assets/teapot.obj");
	if (!mesh0)
		return SafeReturn(1);
	auto teapot = std::make_shared<D3d12Mesh>(mesh0.get(), device.get(), cmd_list);
	if (!*teapot)
		return SafeReturn(1);
	mesh0 = nullptr;

	// set camera
	Transform view;
	Projection proj;
	view.pos = Vector(2, 2, -3);
	view.SetFront(-view.pos);
	proj.aspect = static_cast<float>(width) / height;

	// create cb
	struct CBFrame
	{
		XMFLOAT4X4 view;
		XMFLOAT4X4 inv_view;
		XMFLOAT4X4 proj;
		XMFLOAT3 light_intensity;
		float _pad1;
		XMFLOAT3 light_direction;
		float _pad2;
	};
	IndexLoop cb_i{ 0,1 };
	auto cb = std::make_shared<D3d12UploadBuffer<CBFrame>>(true, 2, device.get());
	if (!*cb)
		return SafeReturn(1);
	CBFrame cbframe{};
	cbframe.view = XmFloat4x4(MatrixTranspose(view.GetInverseTransformMatrix()));
	cbframe.inv_view = XmFloat4x4(MatrixTranspose(view.GetTransformMatrix()));
	cbframe.proj = XmFloat4x4(MatrixTranspose(proj.GetTransformMatrix()));
	Transform light;
	light.axes.pitch = pi_div2 * 0.8f;
	light.scale = Vector(0.8f, 0.8f, 0.8f);
	cbframe.light_intensity = XmFloat3(light.scale);
	cbframe.light_direction = XmFloat3(light.GetFront());
	if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
		return SafeReturn(1);

	// create obj_tfs
	struct CBObj
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 inv_world;
		XMFLOAT4X4 _keep1;
		XMFLOAT4X4 _keep2;
		XMFLOAT4X4 _keep3;
	};
	const UINT instance_count = 10;
	const UINT obj_tfs_srv = 0;
	auto obj_tfs = std::make_shared<D3d12UploadBuffer<CBObj>>(false, instance_count, device.get());
	if (!*obj_tfs)
		return SafeReturn(1);
	srv_desc = obj_tfs->GetBufferObj()->GetSrvDesc(0, instance_count);
	device->Get()->CreateShaderResourceView(obj_tfs->GetBuffer(), &srv_desc, device->GetSrv(obj_tfs_srv));
	Transform obj_tf;
	CBObj cbobjs[instance_count]{};
	int i = 0;
	for (float x = -1; x <= 1; x += 2)
	{
		for (float z = -2; z <= 2; z += 1, i++)
		{
			obj_tf.pos.x = x;
			obj_tf.pos.z = z;
			obj_tf.SetFront(-obj_tf.pos);
			obj_tf.scale = Vector(0.35f, 0.45f, 0.35f);
			cbobjs[i].world = XmFloat4x4(MatrixTranspose(obj_tf.GetTransformMatrix()));
			cbobjs[i].inv_world = XmFloat4x4(MatrixTranspose(obj_tf.GetInverseTransformMatrix()));
		}
	}
	if (!obj_tfs->UploadData(0, cbobjs, instance_count))
		return SafeReturn(1);

	// create shader
	auto vs = D3d12Util::CompileShaderFile(L"vs_transform_all.hlsl", "vs");
	if (!vs)
		return SafeReturn(1);
	auto ps = D3d12Util::CompileShaderFile(L"ps_light.hlsl", "ps");
	if (!ps)
		return SafeReturn(1);

	// create pso
	D3d12GPSOCreator psc;
	psc.SetRSig(device->GetRSig());
	psc.SetShader(vs.Get(), ps.Get());
	psc.SetIA(cube->input_layout);
	psc.SetRenderTargets(target->msaa);
	auto pso = psc.CreatePSO(device->Get());
	if (!pso)
		return SafeReturn(1);

	// create cb2
	struct CBBlur
	{
		float ws[11];
		float _pad1;
	};
	int blur_count = 4;
	auto cb2 = std::make_shared<D3d12UploadBuffer<CBBlur>>(true, 1, device.get());
	if (!*cb2)
		return SafeReturn(1);
	CBBlur cbblur{ 0,0,0,0.0545f,0.2442f,0.4026f,0.2442f,0.0545f,0,0,0 };
	if (!cb2->UploadData(0, &cbblur, 1))
		return SafeReturn(1);

	// create textures
	const int blur_tex1_srv = 1;
	const int blur_tex2_srv = 2;
	const int blur_tex1_uav = 3;
	const int blur_tex2_uav = 4;
	rc_desc = D3d12Util::GetTexture2dRcDesc(width, height, 1, DXGI_FORMAT_B8G8R8A8_UNORM, 1, false, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	auto blur_tex1 = std::make_shared<D3d12Texture>(rc_desc, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	if (!blur_tex1->Get())
		return SafeReturn(1);
	auto blur_tex2 = std::make_shared<D3d12Texture>(rc_desc, D3D12_HEAP_TYPE_DEFAULT, device->Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	if (!blur_tex2->Get())
		return SafeReturn(1);
	srv_desc = blur_tex1->GetSrvDesc();
	device->Get()->CreateShaderResourceView(blur_tex1->Get(), &srv_desc, device->GetSrv(blur_tex1_srv));
	srv_desc = blur_tex2->GetSrvDesc();
	device->Get()->CreateShaderResourceView(blur_tex2->Get(), &srv_desc, device->GetSrv(blur_tex2_srv));
	uav_desc = blur_tex1->GetUavDesc();
	device->Get()->CreateUnorderedAccessView(blur_tex1->Get(), nullptr, &uav_desc, device->GetUav(blur_tex1_uav));
	uav_desc = blur_tex2->GetUavDesc();
	device->Get()->CreateUnorderedAccessView(blur_tex2->Get(), nullptr, &uav_desc, device->GetUav(blur_tex2_uav));

	// create compute shader
	auto cs1 = D3d12Util::CompileShaderFile(L"cs_blur_x.hlsl", "cs");
	if (!cs1)
		return SafeReturn(1);
	auto cs2 = D3d12Util::CompileShaderFile(L"cs_blur_y.hlsl", "cs");
	if (!cs2)
		return SafeReturn(1);

	// create compute pso
	D3d12CPSOCreator cpsc;
	cpsc.SetRSig(device->GetRSig());
	cpsc.SetShader(cs1.Get());
	auto cpso1 = cpsc.CreatePSO(device->Get());
	if (!cpso1)
		return SafeReturn(1);
	cpsc.SetShader(cs2.Get());
	auto cpso2 = cpsc.CreatePSO(device->Get());
	if (!cpso2)
		return SafeReturn(1);

	// finish
	if (!device->ExecuteCommand())
		return SafeReturn(1);
	if (!device->FlushGpu())
		return SafeReturn(1);

	// clean up
	cube->CleanUp();

	cout << "press Q to exit" << endl;
	while (window->CheckWindowState())
	{
		// update
		mouse.Present();
		fps_counter.Present();
		float dt = fps_counter.GetLastDeltatime2();

		float cam_move_dis = 5 * dt;
		float cam_rotate_angle_eu = 0.003f;
		if (KeyIsDown('W'))      view.pos += cam_move_dis * view.GetFrontInGround();
		else if (KeyIsDown('S')) view.pos -= cam_move_dis * view.GetFrontInGround();
		if (KeyIsDown('A'))      view.pos -= cam_move_dis * view.GetRightInGround();
		else if (KeyIsDown('D')) view.pos += cam_move_dis * view.GetRightInGround();
		if (KeyIsDown(VK_SPACE)) view.pos.y += cam_move_dis;
		else if (KeyIsDown(VK_LSHIFT)) view.pos.y -= cam_move_dis;
		if (KeyIsDown(VK_MBUTTON))
		{
			view.axes.pitch += cam_rotate_angle_eu * mouse.GetMoveY();
			view.axes.yaw += cam_rotate_angle_eu * mouse.GetMoveX();
			mouse.SetToPrev();
		}

		/*float obj_move_dis = 4 * dt;
		float obj_rotate_angle = 3 * dt;
		if (KeyIsDown('I'))      world.axes.pitch += obj_rotate_angle;
		else if (KeyIsDown('K')) world.axes.pitch -= obj_rotate_angle;
		if (KeyIsDown('J'))      world.axes.yaw += obj_rotate_angle;
		else if (KeyIsDown('L')) world.axes.yaw -= obj_rotate_angle;
		if (KeyIsDown('F'))      world.pos.x -= obj_move_dis;
		else if (KeyIsDown('H')) world.pos.x += obj_move_dis;
		if (KeyIsDown('T'))      world.pos.y += obj_move_dis;
		else if (KeyIsDown('G')) world.pos.y -= obj_move_dis;
		if (KeyIsDown('R'))      world.pos.z -= obj_move_dis;
		else if (KeyIsDown('Y')) world.pos.z += obj_move_dis;*/

		if (KeyIsDown('Z'))      proj.parallel_projection = false;
		else if (KeyIsDown('X')) proj.parallel_projection = true;

		if (KeyIsDown(VK_ADD))           blur_count = Min(blur_count + 1, 4);
		else if (KeyIsDown(VK_SUBTRACT)) blur_count = Max(blur_count - 1, 0);

		cbframe.view = XmFloat4x4(MatrixTranspose(view.GetInverseTransformMatrix()));
		cbframe.inv_view = XmFloat4x4(MatrixTranspose(view.GetTransformMatrix()));
		cbframe.proj = XmFloat4x4(MatrixTranspose(proj.GetTransformMatrix()));
		cbframe.light_intensity = XmFloat3(light.scale);
		cbframe.light_direction = XmFloat3(light.GetFront());
		if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
			return SafeReturn(1);

		// render
		if (device->CheckCmdAllocator())
		{
			cmd_list = device->ResetCommand();
			if (!cmd_list)
				return SafeReturn(1);

			// clear
			target->ClearRenderTargets(device.get(), cmd_list);

			// set target
			target->SetRenderTargets(device.get(), cmd_list);

			// set pso
			cmd_list->SetPipelineState(pso.Get());

			// set IA
			teapot->SetIA(cmd_list);

			// set root parameter
			device->SetRSigCbvFast(cb->GetBufferObj()->GetGpuLocation(cb_i.UseCurrentIndex()));
			device->SetRSigSrv(device->GetSrvGpu(obj_tfs_srv));

			// draw
			cmd_list->DrawIndexedInstanced(teapot->index_count, instance_count, 0, 0, 0);

			// blur
			target->CopyTarget(0, blur_tex1->Get(), 0, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, cmd_list);
			cmd_list->SetComputeRootSignature(device->GetRSig());
			cmd_list->SetComputeRootConstantBufferView(0, cb2->GetBufferObj()->GetGpuLocation(0));
			for (int j = 0; j < blur_count; j++)
			{
				// x
				rc_barr[0] = D3d12Util::GetTransitionStruct(blur_tex1->Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
				rc_barr[1] = D3d12Util::GetTransitionStruct(blur_tex2->Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
				cmd_list->ResourceBarrier(2, rc_barr);
				cmd_list->SetPipelineState(cpso1.Get());
				cmd_list->SetComputeRootDescriptorTable(2, device->GetSrvGpu(blur_tex1_srv));
				cmd_list->SetComputeRootDescriptorTable(3, device->GetUavGpu(blur_tex2_uav));
				cmd_list->Dispatch((width + 255) / 256, height, 1);
				// y
				rc_barr[0] = D3d12Util::GetTransitionStruct(blur_tex1->Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
				rc_barr[1] = D3d12Util::GetTransitionStruct(blur_tex2->Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
				cmd_list->ResourceBarrier(2, rc_barr);
				cmd_list->SetPipelineState(cpso2.Get());
				cmd_list->SetComputeRootDescriptorTable(2, device->GetSrvGpu(blur_tex2_srv));
				cmd_list->SetComputeRootDescriptorTable(3, device->GetUavGpu(blur_tex1_uav));
				cmd_list->Dispatch(width, (height + 255) / 256, 1);
			}

			// present
			if (!device->ExecuteCommandAndPresent(blur_tex1->Get(), false, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE))
				return SafeReturn(1);

			// refresh
			window->Present();
		}

		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}
	return SafeReturn(0);
}

int main_d3d12_show_image_example()
{
	cout << endl << "create window ..." << endl;
	const std::string title = "d3d12 show image example";
	const int width = 800;
	const int height = 600;
	auto window = std::make_unique<SimpleWindowWithFC>(GetModuleHandle(nullptr), width, height, title);
	if (!window->CheckWindowState())
		return 1;
	Mouse mouse;
	FpsCounterS fps_counter;
	fps_counter.LockFps(0);

	cout << "create d3d12 device ..." << endl;
	auto device = std::make_unique<D3d12Device>();
	auto SafeReturn = [&device](int return_v)
	{
		if (device)
			device->FlushGpu();
		return return_v;
	};
	auto cmd_list = device->Create(window.get());
	if (!cmd_list)
		return SafeReturn(1);

	cout << "setup resource ..." << endl;
	uint image_width, image_height;
	auto image = LoadImageFile(L"assets/NightHz.jpg", image_width, image_height);
	if (image == nullptr)
	{
		cout << "load image error" << endl;
		return SafeReturn(1);
	}
	auto image2 = std::make_unique<uint[]>(static_cast<size_t>(width) * height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int i2 = y * width + x;
			int i = (image_height * y / height) * image_width + image_width * x / width;
			image2[i2] = image[i];
		}
	}
	auto tex = std::make_shared<D3d12DefaultTexture>(image2.get(), width, height, device.get(), cmd_list);
	if (!*tex)
		return SafeReturn(1);
	// finish
	if (!device->ExecuteCommand())
		return SafeReturn(1);
	if (!device->FlushGpu())
		return SafeReturn(1);

	cout << "press Q to exit" << endl;
	while (window->CheckWindowState())
	{
		// update
		mouse.Present();
		fps_counter.Present();
		float dt = fps_counter.GetLastDeltatime2(); (dt);

		// render
		if (device->CheckCmdAllocator())
		{
			cmd_list = device->ResetCommand();
			if (!cmd_list)
				return SafeReturn(1);

			// present
			if (!device->ExecuteCommandAndPresent(tex->GetTexture(), false, tex->rc_init_state, tex->rc_init_state))
				return SafeReturn(1);

			// refresh
			window->Present();
		}
		else
			Sleep(1);

		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}
	return SafeReturn(0);
}

int main_d3d12_cubemap_example()
{
	cout << endl << "create window ..." << endl;
	const std::string title = "d3d12 cubemap example";
	const int width = 800;
	const int height = 600;
	auto window = std::make_unique<SimpleWindowWithFC>(GetModuleHandle(nullptr), width, height, title);
	if (!window->CheckWindowState())
		return 1;
	Mouse mouse;
	FpsCounterS fps_counter;
	fps_counter.LockFps(0);

	cout << "create d3d12 device ..." << endl;
	auto device = std::make_unique<D3d12Device>();
	auto SafeReturn = [&device](int return_v)
	{
		if (device)
			device->FlushGpu();
		return return_v;
	};
	auto cmd_list = device->Create(window.get());
	if (!cmd_list)
		return SafeReturn(1);

	cout << "setup resource ..." << endl;
	std::shared_ptr<Mesh> mesh0;
	std::unique_ptr<uint[]> image, image2;
	D3d12GPSOCreator psc;
	D3D12_RESOURCE_BARRIER rc_barr[4]{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};

	// create target
	auto target = std::make_shared<D3d12RenderTarget>(width, height, device->GetScFormat(), true, true, Color::yellow_green_o, 0, 0, device.get());
	if (!*target)
		return SafeReturn(1);

	// create mesh
	mesh0 = CreateCubeMesh();
	auto cube = std::make_shared<D3d12Mesh>(mesh0.get(), device.get(), cmd_list);
	if (!*cube)
		return SafeReturn(1);
	mesh0 = CreateSphereMesh(50);
	auto sphere = std::make_shared<D3d12Mesh>(mesh0.get(), device.get(), cmd_list);
	if (!*sphere)
		return SafeReturn(1);
	mesh0 = nullptr;

	// set camera
	Transform view;
	Projection proj;
	view.pos = Vector(4, 7, -11);
	view.SetFront(-view.pos);
	proj.aspect = static_cast<float>(width) / height;

	// set light
	Transform light;
	light.axes.pitch = pi_div2 * 0.7f;
	light.axes.yaw = pi_div2 * -0.7f;
	light.scale = Vector(0.8f, 0.8f, 0.8f);

	// create cb
	struct CBFrame
	{
		XMFLOAT4X4 view;
		XMFLOAT4X4 inv_view;
		XMFLOAT4X4 proj;
		XMFLOAT3 light_intensity;
		float _pad1;
		XMFLOAT3 light_direction;
		float _pad2;
		XMFLOAT4X4 world;
		XMFLOAT4X4 inv_world;
		XMFLOAT4 color;
	};
	IndexLoop cb_i{ 0,1,2,3,4,5,6,7,8,9 };
	auto cb = std::make_shared<D3d12UploadBuffer<CBFrame>>(true, 10, device.get());
	if (!*cb)
		return SafeReturn(1);
	CBFrame cbframe{};
	cbframe.view = XmFloat4x4(MatrixTranspose(view.GetInverseTransformMatrix()));
	cbframe.inv_view = XmFloat4x4(MatrixTranspose(view.GetTransformMatrix()));
	cbframe.proj = XmFloat4x4(MatrixTranspose(proj.GetTransformMatrix()));
	cbframe.light_intensity = XmFloat3(light.scale);
	cbframe.light_direction = XmFloat3(light.GetFront());
	if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
		return SafeReturn(1);

	// create skybox texture
	uint skybox_width, skybox_height;
	image = LoadImageFile(L"assets/skybox.png", skybox_width, skybox_height);
	if (image == nullptr)
	{
		cout << "load assets/skybox.png error" << endl;
		return SafeReturn(1);
	}
	skybox_width /= 4;
	skybox_height /= 3;
	image2 = std::make_unique<uint[]>(static_cast<size_t>(skybox_width) * skybox_height * 6);
	for (const auto& ixy : std::vector<std::vector<int>>{ {0,2,1},{1,0,1},{2,1,0},{3,1,2},{4,1,1},{5,3,1} })
	{
		uint* src = image.get() + ixy[2] * skybox_width * skybox_height * 4 + ixy[1] * skybox_width;
		uint* dst = image2.get() + ixy[0] * skybox_width * skybox_height;
		for (uint y = 0; y < skybox_height; y++)
		{
			for (uint x = 0; x < skybox_width; x++)
			{
				int src_i = y * skybox_width * 4 + x;
				int dst_i = y * skybox_width + x;
				dst[dst_i] = src[src_i];
			}
		}
	}
	const UINT skybox_srv = 0;
	auto skybox = std::make_shared<D3d12DefaultTexture>(image2.get(), 4, skybox_width, skybox_height, 6, DXGI_FORMAT_B8G8R8A8_UNORM,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, device.get(), cmd_list);
	if (!*skybox)
		return SafeReturn(1);
	srv_desc = skybox->GetTextureObj()->GetSrvDescForCube();
	device->Get()->CreateShaderResourceView(skybox->GetTexture(), &srv_desc, device->GetSrv(skybox_srv));

	// create shader
	auto vs_skybox = D3d12Util::CompileShaderFile(L"vs_skybox.hlsl", "vs");
	if (!vs_skybox)
		return SafeReturn(1);
	auto ps_skybox = D3d12Util::CompileShaderFile(L"ps_skybox.hlsl", "ps");
	if (!ps_skybox)
		return SafeReturn(1);
	auto gs_envmap = D3d12Util::CompileShaderFile(L"gs_envmap.hlsl", "gs");
	if (!gs_envmap)
		return SafeReturn(1);

	// create pso
	psc.Reset();
	psc.SetRSig(device->GetRSig());
	psc.SetShader(vs_skybox.Get(), ps_skybox.Get());
	psc.SetIA(cube->input_layout);
	psc.pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psc.SetRenderTargets(target->msaa);
	auto pso_skybox = psc.CreatePSO(device->Get());
	if (!pso_skybox)
		return SafeReturn(1);
	psc.SetShader(vs_skybox.Get(), ps_skybox.Get(), nullptr, nullptr, gs_envmap.Get());
	psc.SetRenderTargets(false, 6);
	auto pso_skybox6 = psc.CreatePSO(device->Get());
	if (!pso_skybox6)
		return SafeReturn(1);

	// create cube_tfs
	struct BCubeTf
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 inv_world;
		XMFLOAT4 color;
		XMFLOAT4 _keep1;
		XMFLOAT4 _keep2;
		XMFLOAT4 _keep3;
		XMFLOAT4X4 _keep4;
		XMFLOAT4X4 _keep5;
	};
	const UINT cube_tfs_count = 6;
	const UINT cube_tfs_srv = 1;
	auto cube_tfs = std::make_shared<D3d12UploadBuffer<BCubeTf>>(false, cube_tfs_count, device.get());
	if (!*cube_tfs)
		return SafeReturn(1);
	srv_desc = cube_tfs->GetBufferObj()->GetSrvDesc(0, cube_tfs_count);
	device->Get()->CreateShaderResourceView(cube_tfs->GetBuffer(), &srv_desc, device->GetSrv(cube_tfs_srv));
	BCubeTf bcube_tfs[cube_tfs_count]{};
	const std::vector<Vector> cube_scale{ Vector(8,0.2f,8),  Vector(0.2f,1.5f,5), Vector(0.5f,0.5f,0.5f), Vector(0.5f,0.5f,0.5f),  Vector(0.5f,0.5f,0.5f),   Vector(0.5f,0.5f,0.5f) };
	const std::vector<Vector> cube_pos{ Vector(0,-0.2f,0), Vector(0,1.5f,0),    Vector(5.5f,0.5f,3.5f), Vector(-5.5f,0.5f,3.5f), Vector(-5.5f,0.5f,-3.5f), Vector(5.5f,0.5f,-3.5f) };
	const std::vector<Color> cube_color{ Color::green_l, Color::red_l, Color::purple_l, Color::orange_l, Color::pink_l, Color::yellow_l };
	for (int i = 0; i < cube_tfs_count; i++)
	{
		Transform transform;
		transform.pos = cube_pos[i];
		transform.scale = cube_scale[i];
		bcube_tfs[i].world = XmFloat4x4(MatrixTranspose(transform.GetTransformMatrix()));
		bcube_tfs[i].inv_world = XmFloat4x4(MatrixTranspose(transform.GetInverseTransformMatrix()));
		bcube_tfs[i].color = XmFloat4(cube_color[i]);
	}
	if (!cube_tfs->UploadData(0, bcube_tfs, cube_tfs_count))
		return SafeReturn(1);

	// create shader
	auto vs_transform_all = D3d12Util::CompileShaderFile(L"vs_transform_all.hlsl", "vs");
	if (!vs_transform_all)
		return SafeReturn(1);
	auto ps_light = D3d12Util::CompileShaderFile(L"ps_light.hlsl", "ps");
	if (!ps_light)
		return SafeReturn(1);

	// create pso
	psc.Reset();
	psc.SetRSig(device->GetRSig());
	psc.SetShader(vs_transform_all.Get(), ps_light.Get());
	psc.SetIA(cube->input_layout);
	psc.SetRenderTargets(target->msaa);
	auto pso_cubes = psc.CreatePSO(device->Get());
	if (!pso_cubes)
		return SafeReturn(1);
	psc.SetShader(vs_transform_all.Get(), ps_light.Get(), nullptr, nullptr, gs_envmap.Get());
	psc.SetRenderTargets(false, 6);
	auto pso_cubes6 = psc.CreatePSO(device->Get());
	if (!pso_cubes6)
		return SafeReturn(1);

	// set reflection sphere
	Transform reflection_sphere;
	reflection_sphere.pos = Vector(3.5f, 1.5f, 0);
	Color reflection_sphere_color = Color::white;
	Projection reflection_proj;

	// set refraction sphere
	Transform refraction_sphere;
	refraction_sphere.pos = Vector(-3.5, 1.5f, 0);
	Color refraction_sphere_color = Color::white;

	// create reflection cubemap
	const UINT reflection_cubemap_srv = 2;
	auto reflection_cubemap = std::make_shared<D3d12RenderTarget>(256, 256, DXGI_FORMAT_B8G8R8A8_UNORM, false, static_cast<UINT16>(6), true, Color::black, 1, 1, device.get());
	if (!*reflection_cubemap)
		return SafeReturn(1);
	srv_desc = reflection_cubemap->GetTargetObj()->GetSrvDescForCube();
	device->Get()->CreateShaderResourceView(reflection_cubemap->GetTarget(), &srv_desc, device->GetSrv(reflection_cubemap_srv));

	// create refraction cubemap
	const UINT refraction_cubemap_srv = 3;
	auto refraction_cubemap = std::make_shared<D3d12RenderTarget>(256, 256, DXGI_FORMAT_B8G8R8A8_UNORM, false, static_cast<UINT16>(6), true, Color::black, 2, 2, device.get());
	if (!*refraction_cubemap)
		return SafeReturn(1);
	srv_desc = refraction_cubemap->GetTargetObj()->GetSrvDescForCube();
	device->Get()->CreateShaderResourceView(refraction_cubemap->GetTarget(), &srv_desc, device->GetSrv(refraction_cubemap_srv));

	// create shader
	auto vs_transform_one = D3d12Util::CompileShaderFile(L"vs_transform_one.hlsl", "vs");
	if (!vs_transform_one)
		return SafeReturn(1);
	auto ps_reflection = D3d12Util::CompileShaderFile(L"ps_reflection.hlsl", "ps");
	if (!ps_reflection)
		return SafeReturn(1);
	auto ps_refraction = D3d12Util::CompileShaderFile(L"ps_refraction.hlsl", "ps");
	if (!ps_refraction)
		return SafeReturn(1);

	// create pso
	psc.Reset();
	psc.SetRSig(device->GetRSig());
	psc.SetShader(vs_transform_one.Get(), ps_reflection.Get());
	psc.SetIA(cube->input_layout);
	psc.SetRenderTargets(target->msaa);
	auto pso_reflection = psc.CreatePSO(device->Get());
	if (!pso_reflection)
		return SafeReturn(1);
	psc.SetShader(vs_transform_one.Get(), ps_refraction.Get());
	auto pso_refraction = psc.CreatePSO(device->Get());
	if (!pso_refraction)
		return SafeReturn(1);

	// finish
	if (!device->ExecuteCommand())
		return SafeReturn(1);
	if (!device->FlushGpu())
		return SafeReturn(1);

	cout << "press Q to exit" << endl;
	while (window->CheckWindowState())
	{
		// update
		mouse.Present();
		fps_counter.Present();
		float dt = fps_counter.GetLastDeltatime2();

		float cam_move_dis = 5 * dt;
		float cam_rotate_angle_eu = 0.003f;
		if (KeyIsDown('W'))      view.pos += cam_move_dis * view.GetFrontInGround();
		else if (KeyIsDown('S')) view.pos -= cam_move_dis * view.GetFrontInGround();
		if (KeyIsDown('A'))      view.pos -= cam_move_dis * view.GetRightInGround();
		else if (KeyIsDown('D')) view.pos += cam_move_dis * view.GetRightInGround();
		if (KeyIsDown(VK_SPACE)) view.pos.y += cam_move_dis;
		else if (KeyIsDown(VK_LSHIFT)) view.pos.y -= cam_move_dis;
		if (KeyIsDown(VK_MBUTTON))
		{
			view.axes.pitch += cam_rotate_angle_eu * mouse.GetMoveY();
			view.axes.yaw += cam_rotate_angle_eu * mouse.GetMoveX();
			mouse.SetToPrev();
		}
		if (KeyIsDown(VK_F1))
		{
			view.axes.yaw += 0.3f * dt;
		}
		else if (KeyIsDown(VK_F2))
		{
			view.axes.yaw += 1.2f * dt;
			float d = VectorLength(view.pos - reflection_sphere.pos);
			view.pos = reflection_sphere.pos - d * view.GetFront();
		}
		else if (KeyIsDown(VK_F3))
		{
			view.axes.yaw += 1.2f * dt;
			float d = VectorLength(view.pos - refraction_sphere.pos);
			view.pos = refraction_sphere.pos - d * view.GetFront();
		}

		if (KeyIsDown('Z'))      proj.parallel_projection = false;
		else if (KeyIsDown('X')) proj.parallel_projection = true;

		cbframe.light_intensity = XmFloat3(light.scale);
		cbframe.light_direction = XmFloat3(light.GetFront());

		// render
		if (device->CheckCmdAllocator())
		{
			cmd_list = device->ResetCommand();
			if (!cmd_list)
				return SafeReturn(1);

			// clear
			target->ClearRenderTargets(device.get(), cmd_list);
			reflection_cubemap->ClearRenderTargets(device.get(), cmd_list);
			refraction_cubemap->ClearRenderTargets(device.get(), cmd_list);

			// render reflection cubemap
			reflection_cubemap->SetRenderTargets(device.get(), cmd_list);
			reflection_cubemap->SetRS(cmd_list);
			cbframe.view = XmFloat4x4(MatrixTranspose(reflection_sphere.GetInverseTransformMatrix()));
			cbframe.inv_view = XmFloat4x4(MatrixTranspose(reflection_sphere.GetTransformMatrix()));
			cbframe.proj = XmFloat4x4(MatrixTranspose(reflection_proj.GetTransformMatrix()));
			if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
				return SafeReturn(1);
			device->SetRSigCbvFast(cb->GetBufferObj()->GetGpuLocation(cb_i.UseCurrentIndex()));
			// cubes
			cmd_list->SetPipelineState(pso_cubes6.Get());
			cube->SetIA(cmd_list);
			device->SetRSigSrv(device->GetSrvGpu(cube_tfs_srv));
			cmd_list->DrawIndexedInstanced(cube->index_count, cube_tfs_count, 0, 0, 0);
			// skybox
			cmd_list->SetPipelineState(pso_skybox6.Get());
			sphere->SetIA(cmd_list);
			device->SetRSigSrv(device->GetSrvGpu(skybox_srv));
			cmd_list->DrawIndexedInstanced(sphere->index_count, 1, 0, 0, 0);

			// render refraction cubemap
			refraction_cubemap->SetRenderTargets(device.get(), cmd_list);
			refraction_cubemap->SetRS(cmd_list);
			cbframe.view = XmFloat4x4(MatrixTranspose(refraction_sphere.GetInverseTransformMatrix()));
			cbframe.inv_view = XmFloat4x4(MatrixTranspose(refraction_sphere.GetTransformMatrix()));
			if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
				return SafeReturn(1);
			device->SetRSigCbvFast(cb->GetBufferObj()->GetGpuLocation(cb_i.UseCurrentIndex()));
			// cubes
			cmd_list->SetPipelineState(pso_cubes6.Get());
			cube->SetIA(cmd_list);
			device->SetRSigSrv(device->GetSrvGpu(cube_tfs_srv));
			cmd_list->DrawIndexedInstanced(cube->index_count, cube_tfs_count, 0, 0, 0);
			// skybox
			cmd_list->SetPipelineState(pso_skybox6.Get());
			sphere->SetIA(cmd_list);
			device->SetRSigSrv(device->GetSrvGpu(skybox_srv));
			cmd_list->DrawIndexedInstanced(sphere->index_count, 1, 0, 0, 0);

			// common
			target->SetRenderTargets(device.get(), cmd_list);
			target->SetRS(cmd_list);
			cbframe.view = XmFloat4x4(MatrixTranspose(view.GetInverseTransformMatrix()));
			cbframe.inv_view = XmFloat4x4(MatrixTranspose(view.GetTransformMatrix()));
			cbframe.proj = XmFloat4x4(MatrixTranspose(proj.GetTransformMatrix()));
			if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
				return SafeReturn(1);
			device->SetRSigCbvFast(cb->GetBufferObj()->GetGpuLocation(cb_i.UseCurrentIndex()));

			// skybox
			cmd_list->SetPipelineState(pso_skybox.Get());
			sphere->SetIA(cmd_list);
			device->SetRSigSrv(device->GetSrvGpu(skybox_srv));
			cmd_list->DrawIndexedInstanced(sphere->index_count, 1, 0, 0, 0);

			// cubes
			cmd_list->SetPipelineState(pso_cubes.Get());
			cube->SetIA(cmd_list);
			device->SetRSigSrv(device->GetSrvGpu(cube_tfs_srv));
			cmd_list->DrawIndexedInstanced(cube->index_count, cube_tfs_count, 0, 0, 0);

			// reflection sphere
			cmd_list->SetPipelineState(pso_reflection.Get());
			sphere->SetIA(cmd_list);
			cbframe.world = XmFloat4x4(MatrixTranspose(reflection_sphere.GetTransformMatrix()));
			cbframe.inv_world = XmFloat4x4(MatrixTranspose(reflection_sphere.GetInverseTransformMatrix()));
			cbframe.color = XmFloat4(reflection_sphere_color);
			if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
				return SafeReturn(1);
			device->SetRSigCbvFast(cb->GetBufferObj()->GetGpuLocation(cb_i.UseCurrentIndex()));
			rc_barr[0] = D3d12Util::GetTransitionStruct(reflection_cubemap->GetTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			cmd_list->ResourceBarrier(1, rc_barr);
			device->SetRSigSrv(device->GetSrvGpu(reflection_cubemap_srv));
			cmd_list->DrawIndexedInstanced(sphere->index_count, 1, 0, 0, 0);
			rc_barr[0] = D3d12Util::GetTransitionStruct(reflection_cubemap->GetTarget(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			cmd_list->ResourceBarrier(1, rc_barr);

			// refraction sphere
			cmd_list->SetPipelineState(pso_refraction.Get());
			sphere->SetIA(cmd_list);
			cbframe.world = XmFloat4x4(MatrixTranspose(refraction_sphere.GetTransformMatrix()));
			cbframe.inv_world = XmFloat4x4(MatrixTranspose(refraction_sphere.GetInverseTransformMatrix()));
			cbframe.color = XmFloat4(refraction_sphere_color);
			if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
				return SafeReturn(1);
			device->SetRSigCbvFast(cb->GetBufferObj()->GetGpuLocation(cb_i.UseCurrentIndex()));
			rc_barr[0] = D3d12Util::GetTransitionStruct(refraction_cubemap->GetTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			cmd_list->ResourceBarrier(1, rc_barr);
			device->SetRSigSrv(device->GetSrvGpu(refraction_cubemap_srv));
			cmd_list->DrawIndexedInstanced(sphere->index_count, 1, 0, 0, 0);
			rc_barr[0] = D3d12Util::GetTransitionStruct(refraction_cubemap->GetTarget(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
			cmd_list->ResourceBarrier(1, rc_barr);

			// present
			if (!device->ExecuteCommandAndPresent(target->GetTarget(), target->msaa))
				return SafeReturn(1);

			// refresh
			window->Present();
		}
		else
			Sleep(1);

		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}
	return SafeReturn(0);
}

int main_d3d12_shadow_example()
{
	cout << endl << "create window ..." << endl;
	const std::string title = "d3d12 shadow example";
	const int width = 800;
	const int height = 600;
	auto window = std::make_unique<SimpleWindowWithFC>(GetModuleHandle(nullptr), width, height, title);
	if (!window->CheckWindowState())
		return 1;
	Mouse mouse;
	FpsCounterS fps_counter;
	fps_counter.LockFps(0);

	cout << "create d3d12 device ..." << endl;
	auto device = std::make_unique<D3d12Device>();
	auto SafeReturn = [&device](int return_v)
	{
		if (device)
			device->FlushGpu();
		return return_v;
	};
	auto cmd_list = device->Create(window.get());
	if (!cmd_list)
		return SafeReturn(1);

	cout << "setup resource ..." << endl;
	std::shared_ptr<Mesh> mesh0;
	D3d12GPSOCreator psc;
	D3D12_RESOURCE_BARRIER rc_barr[4]{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
	D3D12_SAMPLER_DESC samp_desc{};

	// create target
	auto target = std::make_shared<D3d12RenderTarget>(width, height, device->GetScFormat(), true, true, Color::yellow_green_o, 0, 0, device.get());
	if (!*target)
		return SafeReturn(1);

	// create mesh
	mesh0 = CreateCubeMesh();
	auto cube = std::make_shared<D3d12Mesh>(mesh0.get(), device.get(), cmd_list);
	if (!*cube)
		return SafeReturn(1);
	mesh0 = nullptr;

	// set camera
	Transform view;
	Projection proj;
	view.pos = Vector(4, 7, -11);
	view.SetFront(-view.pos);
	proj.aspect = static_cast<float>(width) / height;

	// set light
	Transform light;
	light.axes.pitch = pi_div2 * 0.7f;
	light.axes.yaw = pi_div2 * -0.7f;
	light.scale = Vector(0.8f, 0.8f, 0.8f);
	Projection light_proj;
	light_proj.parallel_projection = true;
	light_proj.height = 30;
	light_proj.z_far = 100;

	// create cb
	struct CBFrame
	{
		XMFLOAT4X4 view;
		XMFLOAT4X4 inv_view;
		XMFLOAT4X4 proj;
		XMFLOAT3 light_intensity;
		float _pad1;
		XMFLOAT3 light_direction;
		float _pad2;
		XMFLOAT4X4 _pad3;
		XMFLOAT4X4 _pad4;
		XMFLOAT4 _pad5;
		XMFLOAT4X4 light_view_proj_tex;
	};
	IndexLoop cb_i{ 0,1,2,3,4,5,6,7,8,9 };
	auto cb = std::make_shared<D3d12UploadBuffer<CBFrame>>(true, 10, device.get());
	if (!*cb)
		return SafeReturn(1);
	CBFrame cbframe{};
	cbframe.view = XmFloat4x4(MatrixTranspose(view.GetInverseTransformMatrix()));
	cbframe.inv_view = XmFloat4x4(MatrixTranspose(view.GetTransformMatrix()));
	cbframe.proj = XmFloat4x4(MatrixTranspose(proj.GetTransformMatrix()));
	cbframe.light_intensity = XmFloat3(light.scale);
	cbframe.light_direction = XmFloat3(light.GetFront());
	if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
		return SafeReturn(1);

	// create cube_infos
	struct BObjInfo
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 inv_world;
		XMFLOAT4 color;
		XMFLOAT4 _keep1;
		XMFLOAT4 _keep2;
		XMFLOAT4 _keep3;
		XMFLOAT4X4 _keep4;
		XMFLOAT4X4 _keep5;
	};
	const UINT cube_infos_count = 6;
	const UINT cube_infos_srv = 0;
	auto cube_infos = std::make_shared<D3d12UploadBuffer<BObjInfo>>(false, cube_infos_count, device.get());
	if (!*cube_infos)
		return SafeReturn(1);
	srv_desc = cube_infos->GetBufferObj()->GetSrvDesc(0, cube_infos_count);
	device->Get()->CreateShaderResourceView(cube_infos->GetBuffer(), &srv_desc, device->GetSrv(cube_infos_srv));
	BObjInfo bcube_infos[cube_infos_count]{};
	const std::vector<Vector> cube_scale{ Vector(20,0.2f,20), Vector(0.5f,1.5f,0.5f), Vector(0.5f,1.5f,0.5f), Vector(0.5f,1.5f,0.5f),  Vector(0.5f,1.5f,0.5f),   Vector(0.5f,1.5f,0.5f) };
	const std::vector<Vector> cube_pos{ Vector(0,-0.2f,0),    Vector(0,1.5f,0),       Vector(5.5f,1.5f,3.5f), Vector(-5.5f,1.5f,3.5f), Vector(-5.5f,1.5f,-3.5f), Vector(5.5f,1.5f,-3.5f) };
	const std::vector<Color> cube_color{ Color::green_l, Color::red_l, Color::purple_l, Color::orange_l, Color::pink_l, Color::yellow_l };
	for (int i = 0; i < cube_infos_count; i++)
	{
		Transform transform;
		transform.pos = cube_pos[i];
		transform.scale = cube_scale[i];
		bcube_infos[i].world = XmFloat4x4(MatrixTranspose(transform.GetTransformMatrix()));
		bcube_infos[i].inv_world = XmFloat4x4(MatrixTranspose(transform.GetInverseTransformMatrix()));
		bcube_infos[i].color = XmFloat4(cube_color[i]);
	}
	if (!cube_infos->UploadData(0, bcube_infos, cube_infos_count))
		return SafeReturn(1);

	// create shader
	auto vs_transform_all = D3d12Util::CompileShaderFile(L"vs_transform_all.hlsl", "vs");
	if (!vs_transform_all)
		return SafeReturn(1);
	auto ps_shadow = D3d12Util::CompileShaderFile(L"ps_shadow.hlsl", "ps", { {"BIGPCF","1"} });
	if (!ps_shadow)
		return SafeReturn(1);
	auto ps_color = D3d12Util::CompileShaderFile(L"ps_color.hlsl", "ps");
	if (!ps_color)
		return SafeReturn(1);

	// create pso
	psc.Reset();
	psc.SetRSig(device->GetRSig());
	psc.SetShader(vs_transform_all.Get(), ps_shadow.Get());
	psc.SetIA(cube->input_layout);
	psc.SetRenderTargets(target->msaa);
	auto pso_cubes = psc.CreatePSO(device->Get());
	if (!pso_cubes)
		return SafeReturn(1);
	psc.SetShader(vs_transform_all.Get(), ps_color.Get());
	psc.pso_desc.RasterizerState.DepthBias = static_cast<INT>(0.001f * 0x1000000);
	psc.pso_desc.RasterizerState.DepthBiasClamp = 0.02f;
	psc.pso_desc.RasterizerState.SlopeScaledDepthBias = 1;
	psc.SetRenderTargets(false);
	auto pso_cubes_d = psc.CreatePSO(device->Get());
	if (!pso_cubes_d)
		return SafeReturn(1);

	// create shadow map
	const UINT shadow_map_srv = cube_infos_srv + 1;
	const UINT shadow_map_sampler = 0;
	auto shadow_map = std::make_shared<D3d12RenderTarget>(1024, 1024, device->GetScFormat(), false, true, Color::black, 1, 1, device.get());
	if (!*shadow_map)
		return SafeReturn(1);
	srv_desc = shadow_map->GetZbufferObj()->GetSrvDesc();
	srv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	device->Get()->CreateShaderResourceView(shadow_map->GetZbuffer(), &srv_desc, device->GetSrv(shadow_map_srv));
	samp_desc = D3d12Util::GetComparisonSamplerDesc(D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_COMPARISON_FUNC_GREATER, 1, std::vector<float>{1, 0, 0, 0});
	device->Get()->CreateSampler(&samp_desc, device->GetSampler(shadow_map_sampler));

	// finish
	if (!device->ExecuteCommand())
		return SafeReturn(1);
	if (!device->FlushGpu())
		return SafeReturn(1);

	cout << "press Q to exit" << endl;
	while (window->CheckWindowState())
	{
		// update
		mouse.Present();
		fps_counter.Present();
		float dt = fps_counter.GetLastDeltatime2();

		float cam_move_dis = 5 * dt;
		float cam_rotate_angle_eu = 0.003f;
		if (KeyIsDown('W'))      view.pos += cam_move_dis * view.GetFrontInGround();
		else if (KeyIsDown('S')) view.pos -= cam_move_dis * view.GetFrontInGround();
		if (KeyIsDown('A'))      view.pos -= cam_move_dis * view.GetRightInGround();
		else if (KeyIsDown('D')) view.pos += cam_move_dis * view.GetRightInGround();
		if (KeyIsDown(VK_SPACE)) view.pos.y += cam_move_dis;
		else if (KeyIsDown(VK_LSHIFT)) view.pos.y -= cam_move_dis;
		if (KeyIsDown(VK_MBUTTON))
		{
			view.axes.pitch += cam_rotate_angle_eu * mouse.GetMoveY();
			view.axes.yaw += cam_rotate_angle_eu * mouse.GetMoveX();
			mouse.SetToPrev();
		}

		if (KeyIsDown('Z'))      proj.parallel_projection = false;
		else if (KeyIsDown('X')) proj.parallel_projection = true;

		Vector2 light_rotate_angle = Vector2(3, 3) * dt;
		if (KeyIsDown('I'))      light.axes.pitch += light_rotate_angle.y;
		else if (KeyIsDown('K')) light.axes.pitch -= light_rotate_angle.y;
		if (KeyIsDown('J'))      light.axes.yaw -= light_rotate_angle.x;
		else if (KeyIsDown('L')) light.axes.yaw += light_rotate_angle.x;
		light.pos = -light.GetFront() * light_proj.z_far / 2;

		cbframe.light_intensity = XmFloat3(light.scale);
		cbframe.light_direction = XmFloat3(light.GetFront());
		const Matrix tex_tf(0.5f, 0, 0, 0, 0, -0.5f, 0, 0, 0, 0, 1, 0, 0.5f, 0.5f, 0, 1);
		cbframe.light_view_proj_tex = XmFloat4x4(MatrixTranspose(light.GetInverseTransformMatrix() * light_proj.GetTransformMatrix() * tex_tf));

		// render
		if (device->CheckCmdAllocator())
		{
			cmd_list = device->ResetCommand();
			if (!cmd_list)
				return SafeReturn(1);

			// clear
			target->ClearRenderTargets(device.get(), cmd_list);
			shadow_map->ClearRenderTargets(device.get(), cmd_list);

			// render shadow map
			shadow_map->SetRenderTargets(device.get(), cmd_list);
			shadow_map->SetRS(cmd_list);
			cbframe.view = XmFloat4x4(MatrixTranspose(light.GetInverseTransformMatrix()));
			cbframe.inv_view = XmFloat4x4(MatrixTranspose(light.GetTransformMatrix()));
			cbframe.proj = XmFloat4x4(MatrixTranspose(light_proj.GetTransformMatrix()));
			if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
				return SafeReturn(1);
			device->SetRSigCbvFast(cb->GetBufferObj()->GetGpuLocation(cb_i.UseCurrentIndex()));
			// cubes
			cmd_list->SetPipelineState(pso_cubes_d.Get());
			cube->SetIA(cmd_list);
			device->SetRSigSrv(device->GetSrvGpu(cube_infos_srv));
			cmd_list->DrawIndexedInstanced(cube->index_count, cube_infos_count, 0, 0, 0);

			rc_barr[0] = D3d12Util::GetTransitionStruct(shadow_map->GetZbuffer(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			cmd_list->ResourceBarrier(1, rc_barr);

			// common
			target->SetRenderTargets(device.get(), cmd_list);
			target->SetRS(cmd_list);
			cbframe.view = XmFloat4x4(MatrixTranspose(view.GetInverseTransformMatrix()));
			cbframe.inv_view = XmFloat4x4(MatrixTranspose(view.GetTransformMatrix()));
			cbframe.proj = XmFloat4x4(MatrixTranspose(proj.GetTransformMatrix()));
			if (!cb->UploadData(cb_i.GetCurrentIndex(), &cbframe, 1))
				return SafeReturn(1);
			device->SetRSigCbvFast(cb->GetBufferObj()->GetGpuLocation(cb_i.UseCurrentIndex()));

			// cubes
			cmd_list->SetPipelineState(pso_cubes.Get());
			cube->SetIA(cmd_list);
			device->SetRSigSrv(device->GetSrvGpu(cube_infos_srv));
			device->SetRSigSampler(device->GetSamplerGpu(shadow_map_sampler));
			cmd_list->DrawIndexedInstanced(cube->index_count, cube_infos_count, 0, 0, 0);

			rc_barr[0] = D3d12Util::GetTransitionStruct(shadow_map->GetZbuffer(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			cmd_list->ResourceBarrier(1, rc_barr);

			// present
			if (!device->ExecuteCommandAndPresent(target->GetTarget(), target->msaa))
				return SafeReturn(1);

			// refresh
			window->Present();
		}
		else
			Sleep(1);

		// msg
		SimpleMessageProcess();
		// exit
		if (KeyIsDown('Q'))
			break;
	}
	return SafeReturn(0);
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
	//return main_image_reader_test();
	//return main_d3d12_example();
	//return main_d3d12_show_image_example();
	return main_d3d12_cubemap_example();
	//return main_d3d12_shadow_example();
}