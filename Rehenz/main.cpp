#include <iostream>
using std::cout;
using std::endl;
#include "rehenz.h"
using namespace Rehenz;

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

	return 0;
}