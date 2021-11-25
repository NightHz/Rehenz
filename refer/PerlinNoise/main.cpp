// main.cpp
// 
// By NightHz
// 
// Created in 20180709
//

#define WIN32_LEAN_AND_MEAN

#include "Window.h"
#include "D2DDevice.h"
#include "DInput.h"

#include "Timer.h"
#include "PrintfAsyn.h"

#include <stdio.h>
#include <math.h>


ID2D1HwndRenderTarget * hrt = nullptr;
int width, height;

ID2D1SolidColorBrush * brush;
ID2D1SolidColorBrush * brushRed;


float Interpolate(float a, float b, float t)
{
	return a + (b - a)*t;
}

float Noise(int id, int x)
{
	x = (int)pow(x << 13, x);
	switch (id)
	{
	case 0:
		return (float)(1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
	case 1:
		return (float)(1.0 - ((x * (x * x * 15017 + 789731) + 1376315201) & 0x7fffffff) / 1073740807.0);
	case 2:
		return (float)(1.0 - ((x * (x * x * 15227 + 789511) + 1376314663) & 0x7fffffff) / 1073742913.0);
	case 3:
		return (float)(1.0 - ((x * (x * x * 15373 + 781897) + 1376316241) & 0x7fffffff) / 1073743159.0);
	case 4:
		return (float)(1.0 - ((x * (x * x * 15823 + 783119) + 1376316419) & 0x7fffffff) / 1073745737.0);
	default:
		return (float)(1.0 - ((x * (x * x * 15559 + 783707) + 1376319823) & 0x7fffffff) / 1073748659.0);
	}
}

float InterpolatedNoise(int id, float x)
{
	int i = (int)x;
	float f = x - i;
	return Interpolate(Noise(id, i), Noise(id, i + 1), f);
}

float PerlinNoise(float x)
{
	float total = 0;

	float persistence = 1.0 / 2;

	float frequency;
	float amplitude;

	for (int i = 0; i<5; i++)
	{
		frequency = pow(2, i);
		amplitude = pow(persistence, i);
		total +=InterpolatedNoise(i, x*frequency)*amplitude;
		if (amplitude < 1e-5)
			break;
	}
	return total;
}


bool Run()
{
	return true;
}

void Init()
{
	hrt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
	hrt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brushRed);
}

void DrawPoint(int _x, int _y)
{
	D2D1_POINT_2F point1 = D2D1::Point2F((float)_x, (float)_y);
	D2D1_POINT_2F point2 = D2D1::Point2F((float)_x+2, (float)_y);
	hrt->DrawLine(point1, point2, brush);
}
void DrawRedPoint(int _x, int _y)
{
	D2D1_POINT_2F point1 = D2D1::Point2F((float)_x, (float)_y);
	D2D1_POINT_2F point2 = D2D1::Point2F((float)_x + 2, (float)_y);
	hrt->DrawLine(point1, point2, brushRed);
}
void DrawLine(int _x1, int _y1, int _x2, int _y2)
{
	D2D1_POINT_2F point1 = D2D1::Point2F((float)_x1, (float)_y1);
	D2D1_POINT_2F point2 = D2D1::Point2F((float)_x2, (float)_y2);
	hrt->DrawLine(point1, point2, brush);
}

bool Draw()
{
	hrt->BeginDraw();
	hrt->Clear();

	DrawLine(width / 2, 0, width / 2, height);
	DrawLine(0, height / 2, width, height / 2);

	float scalex = 100;
	float scaley = 100;
	for (int id = 0; id < 5; id++)
	{
		for (int x = 0; x < width; x++)
		{
			int y = height / 2 - InterpolatedNoise(id, (x - width / 2) / scalex)*scaley;
			DrawRedPoint(x, y);
		}
	}
	for (int x = 0; x < width; x++)
	{
		int y = height / 2 - PerlinNoise((x - width / 2) / scalex)*scaley;
		DrawPoint(x, y);
	}

	return SUCCEEDED(hrt->EndDraw());
}

int SafeExit()
{
	PrintfAsynFlush();
	char c;
	scanf_s("%c", &c, 1);
	return 0;
}

int main()
{
	if (!CreatePrintfAsynThread())
		return 0;

	Window window;
	Window::InitWindowClass(GetModuleHandle(nullptr));
	window.QuickSetting(WINDOW_MODE_GAME_WINDOWED);
	window.Create(L"PerlinNoise");
	window.GetSize(width, height);

	D2DDevice d2dDevice;
	d2dDevice.Create(window.GetHwnd(), &hrt);

	DInput * dInput = DInput::GetInstance();
	dInput->Create(window.GetHinstance(), window.GetHwnd());

	Init();

	while (true)
	{
		Window::MessageLoop();
		dInput->UpdateKeyboardState();

		if (dInput->GetKeyboardState(DIK_ESCAPE))
			break;
		else if (!window.IsOpen())
			break;

		if (!Run())
			break;
		if (!Draw())
		{
			PrintfAsyn("Draw Error!\n");
			break;
		}
	}

	PrintfAsyn("Close\n");
	return SafeExit();
}
