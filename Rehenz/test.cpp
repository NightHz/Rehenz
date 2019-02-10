// test.cpp
//

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include "HPCG.h"
#include "Windows/CSurface.h"

int perlin1DTest()
{
	int seed;
	printf("Input seed : ");
	scanf_s("%d", &seed);

	Rehenz::Windows::CSurface sf;
	sf.Create(nullptr, 600, 400, "Test");
	UINT * image = new UINT[400 * 600];

	memset(image, 0, sizeof(UINT) * 400 * 600);
	for (int x = 0; x < 600; x++)
	{
		image[200 * 600 + x] = 0x00ff00;
		image[50 * 600 + x]  = 0xffff00;
		image[350 * 600 + x] = 0xffff00;
		float value = Rehenz::PerlinOctave(seed, x / 100.0f, 5, 0.5);
		int y = (int)(200 + 150 * value);
		image[y * 600 + x] = x % 100 == 0 ? 0xff0000 : 0xffffff;
	}
	sf.FillFromImage(image);

	bool state = true;
	while (true)
	{
		sf.Present(state);
		if (!state)
			break;
	}

	delete[] image;
	sf.Destroy();
	return 0;
}

int perlin2DTest()
{
	int seed;
	printf("Input seed : ");
	scanf_s("%d", &seed);

	Rehenz::Windows::CSurface sf;
	sf.Create(nullptr, 600, 600, "Test");
	UINT * image = new UINT[600 * 600];

	memset(image, 0, sizeof(UINT) * 600 * 600);
	UINT * p = image;
	printf("time: %d\n", clock());
	for (int y = 0; y < 600; y++)
	{
		for (int x = 0; x < 600; x++)
		{
			//float value = Rehenz::PerlinOctave(seed, x / 166.0f, y / 100.0f, 3, 0.5);
			float value = Rehenz::Perlin(seed, x / 166.0f, y / 100.0f);
			UINT v2 = (UINT)(0xff * (value < 0 ? -value : value));
			UINT color = (value < 0 ? (v2 << 8) + (v2 << 16) : (v2 << 8));
			/*if (x % 100 == 0 && y % 100 == 0)
			{
				p[x] = 0xff0000;
				printf("x = %d \t y = %d \t value = %f \t color = 0x%x \n", x, y, value, color);
			}
			else*/
				p[x] = color;
		}
		p += 600;
	}
	printf("time: %d\n", clock());
	sf.FillFromImage(image);

	bool state = true;
	while (true)
	{
		sf.Present(state);
		if (!state)
			break;
	}

	delete[] image;
	sf.Destroy();
	return 0;
}

int main()
{
	return perlin2DTest();
}
