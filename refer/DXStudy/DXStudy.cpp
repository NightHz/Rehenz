//DXStudy.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include <D2D1.h>


#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // instance
HWND hWnd;                                      // window
WCHAR szTitle[MAX_LOADSTRING];                  // title string
WCHAR szWindowClass[MAX_LOADSTRING];            // window class string
const int windowWidth  = 800;
const int windowHeight = 600;
int appState = 0;

ID2D1Factory *p_d2d_factory = nullptr;
ID2D1HwndRenderTarget *p_d2d_hrt = nullptr;
ID2D1SolidColorBrush * p_brush = nullptr;


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitWindow(HINSTANCE, int);
BOOL                InitD2D();
BOOL                AppMainLoop();
void                Shutdown();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // init sring
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DXSTUDY, szWindowClass, MAX_LOADSTRING);

	// register class
    MyRegisterClass(hInstance);

    // init app
    if (!InitWindow (hInstance, nCmdShow))
    {
        return FALSE;
    }
	if (!InitD2D())
	{
		return FALSE;
	}

    // main messages loop
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (!AppMainLoop())
		{
			return FALSE;
		}
	}

	Shutdown();

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DXSTUDY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DXSTUDY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitWindow(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
	   CW_USEDEFAULT, 0, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
// BOOL InitD2D()
//
// create d2d factory, a render target and a brush
//
BOOL InitD2D()
{
	HRESULT hr;
	RECT rc;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &p_d2d_factory);
	if (FAILED(hr))
		return FALSE;

	GetClientRect(hWnd, &rc);
	hr = p_d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &p_d2d_hrt);
	if (FAILED(hr))
		return FALSE;

	hr = p_d2d_hrt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &p_brush);
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//
// BOOL AppMainLoop()
//
// Application Main Loop
//
BOOL AppMainLoop()
{
	static HRESULT hr;
	static RECT rc;

	GetClientRect(hWnd, &rc);
	p_d2d_hrt->BeginDraw();
	p_d2d_hrt->DrawRectangle(D2D1::RectF(rc.left + 50.0f, rc.top + 100.0f, rc.right - 150.0f, rc.bottom - 100.0f), p_brush);
	hr = p_d2d_hrt->EndDraw();
	if (FAILED(hr))
		return FALSE;

	return TRUE;
}

//
// void Shutdow()
//
// shudown application
//
void Shutdown()
{
	p_brush->Release();
	p_brush = nullptr;
	p_d2d_hrt->Release();
	p_d2d_hrt = nullptr;
	p_d2d_factory->Release();
	p_d2d_factory = nullptr;
	return;
}