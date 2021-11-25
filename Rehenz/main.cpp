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
	srf_dx8.Create(GetModuleHandle(nullptr), 800, 600, title.c_str());
	int fps[2] = { 0,0 };
	auto fps_t0 = timeGetTime();
	auto t0 = timeGetTime();
	while (srf_dx8.GetWindowState())
	{
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