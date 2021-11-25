//D3DStudy.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "CDevice.h"
#include "resource.h"

#define MAX_LOADSTRING 100


LPCWSTR szTitle = L"D3DStudy";         // 标题栏文本
LPCWSTR szWindowClass = L"D3DSTUDY";            // 主窗口类名

const int width = 640;
const int height = 480;
const DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;

CDevice device;

// 此代码模块中包含的函数的前向声明: 
BOOL                InitInstance(HINSTANCE, HWND&, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	HWND hWnd;
    if (!InitInstance (hInstance, hWnd, nCmdShow))
    {
		OutputDebugStringA("<*> InitInstance() -- error\n");
        return FALSE;
    }

	if (!device.CreateDevice(hInstance, hWnd, width, height))
	{
		OutputDebugStringA("<*> device.CreateDevice() -- error\n");
		return FALSE;
	}

    // 主消息循环: 
    MSG msg;
    while (true)
    {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				OutputDebugStringA("<*> Get a WM_QUIT message\n");
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (!device.MainLoop())
		{
			OutputDebugStringA("<*> device.MainLoop() -- error\n");
			return FALSE;
		}
    }

	device.FreeResource();

	OutputDebugStringA("<*> Normal exit\n");
    return (int) msg.wParam;
}

//
//
//
//
//
BOOL InitInstance(HINSTANCE hInstance, HWND &hWnd, int nCmdShow)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D3DSTUDY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_D3DSTUDY);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, windowStyle, false);

	hWnd = CreateWindowW(szWindowClass, szTitle, windowStyle,
		CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
            // 分析菜单选择: 
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


