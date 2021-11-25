// D2DDevice.h
//
// By NightHz
//

#define WIN32_LEAN_AND_MEAN

#include "D2DDevice.h"

#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "dxguid.lib")



D2DDevice::D2DDevice()
{
	state = false;
}


D2DDevice::~D2DDevice()
{
}

bool D2DDevice::Create(HWND _hwnd, ID2D1HwndRenderTarget ** _pHrt)
{
	if (state)
		return false;

	hwnd = _hwnd;

	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (FAILED(hr))
	{
		return false;
	}

	RECT rect;
	if (!GetClientRect(hwnd, &rect))
	{
		factory->Release();
		return false;
	}

	hr = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd,
			D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)),
		&hrt);
	if (FAILED(hr))
	{
		factory->Release();
		return false;
	}
	
	state = true;
	(*_pHrt) = hrt;
	return true;
}

void D2DDevice::Close()
{
	if (!state)
		return;

	hrt->Release();
	factory->Release();

	state = false;
}


bool D2DDevice::Test()
{
	if (!state)
		return false;

	ID2D1SolidColorBrush * redBrush;

	hrt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &redBrush);


	D2D_RECT_F rect;
	rect.left = 0;
	rect.right = 900;
	rect.top = 50;
	rect.bottom = 130;

	hrt->BeginDraw();
	hrt->SetTransform(
		D2D1::Matrix3x2F::Rotation(
			0,
			D2D1::Point2F(468.0f, 331.5f))
	);
	hrt->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
	hrt->DrawRectangle(&rect, redBrush);
	hrt->SetTransform(
		D2D1::Matrix3x2F::Rotation(
			45.0f,
			D2D1::Point2F(468.0f, 331.5f))
	);
	hrt->DrawRectangle(&rect, redBrush);
	hrt->EndDraw();

	redBrush->Release();

	return true;
}