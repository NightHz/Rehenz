#include "window.h"
#include <timeapi.h>

#define WINDOW_CLASS_NAME TEXT("REHENZ")

namespace Rehenz
{
	bool SimpleWindow::InitDefaultWindowClass(HINSTANCE hinstance)
	{
		WNDCLASSEX winclass;
		if (GetClassInfoEx(hinstance, WINDOW_CLASS_NAME, &winclass))
			return true;
		winclass.cbSize = sizeof(WNDCLASSEX);
		winclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		winclass.lpszClassName = WINDOW_CLASS_NAME;
		winclass.lpfnWndProc = DefWindowProc;
		winclass.cbClsExtra = 0;
		winclass.cbWndExtra = 0;
		winclass.hInstance = hinstance;
		winclass.hIcon = nullptr;
		winclass.hIconSm = nullptr;
		winclass.hCursor = nullptr;
		winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		winclass.lpszMenuName = nullptr;
		if (!RegisterClassEx(&winclass))
			return false;
		return true;
	}

	SimpleWindow::SimpleWindow(HINSTANCE _hinstance, int _width, int _height, const String& _title)
		: hinstance(_hinstance), hwnd(nullptr), width(_width), height(_height)
	{
		SetTitle(_title);

		if (!InitDefaultWindowClass(hinstance))
		{
			hwnd = nullptr;
			return;
		}

		RECT rc;
		SetRect(&rc, 0, 0, width, height);
		DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
		AdjustWindowRectEx(&rc, style, false, 0);
		hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME, title.c_str(), style,
			0, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hinstance, nullptr);
	}

	SimpleWindow::~SimpleWindow()
	{
		if (hwnd)
		{
			DestroyWindow(hwnd);
			hwnd = nullptr;
		}
	}

	bool SimpleWindow::CheckWindowState()
	{
		if (!hwnd)
			return false;

		HWND hwnd_find = nullptr;
		while (true)
		{
			hwnd_find = FindWindowEx(nullptr, hwnd_find, WINDOW_CLASS_NAME, title.c_str());
			if (!hwnd_find)
			{
				return false;
			}
			else if (hwnd_find == hwnd)
			{
				return true;
			}
			else
				continue;
		}
		return false;
	}

	void SimpleWindow::SetTitle(const String& _title)
	{
		title = _title;
		if (hwnd)
			SetWindowText(hwnd, title.c_str());
	}

	void SimpleMessageProcess()
	{
		MSG msg;
		while (true)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				break;
		}
	}

	SimpleWindowWithFC::SimpleWindowWithFC(HINSTANCE _hinstance, int _width, int _height, const String& _title_base)
		: SimpleWindow(_hinstance, _width, _height, _title_base), title_base(_title_base), fps_counter(timeGetTime)
	{
		fps_counter.LockFps(100);
		auto updateFps = [this](Rehenz::uint fps)
		{
			this->SetTitle((title_base + " fps:" + std::to_string(fps)).c_str());
		};
		fps_counter.UpdateFpsCallback = updateFps;
	}

	SimpleWindowWithFC::~SimpleWindowWithFC()
	{
	}

	void SimpleWindowWithFC::Present()
	{
		fps_counter.Present();
	}
}