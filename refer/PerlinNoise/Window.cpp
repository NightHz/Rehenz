// Window.h
//
// By NightHz
//
// Build a window.

#define WIN32_LEAN_AND_MEAN

#include "Window.h"
#include <stdio.h>



bool Window::stateClass = false;
HINSTANCE Window::hinstance = nullptr;

#ifdef UNICODE
static const wchar_t className[] = L"windowClassName";
#else
static const char className[] = "windowClassName";
#endif


struct WindowList
{
	Window * window;
	WindowList * next;
};

static WindowList * windowListHead = nullptr;
static WindowList * windowListTail = nullptr;

void WindowListAdd(Window *_window)
{
	if (windowListHead == nullptr)
	{
		windowListHead = new WindowList;
		windowListHead->window = _window;
		windowListHead->next = nullptr;
		windowListTail = windowListHead;
	}
	else
	{
		windowListTail->next = new WindowList;
		windowListTail = windowListTail->next;
		windowListTail->window = _window;
		windowListTail->next = nullptr;
	}
}

Window * WindowListRemove(HWND _hwnd)
{
	WindowList * pUnit = windowListHead;
	WindowList * pPreUnit = nullptr;
	while (pUnit)
	{
		if (pUnit->window->GetHwnd() == _hwnd)
		{
			if (pPreUnit == nullptr)
				windowListHead = pUnit->next;
			else
				pPreUnit->next = pUnit->next;
			if (pUnit->next == nullptr)
				windowListTail = pPreUnit;
			Window * window = pUnit->window;
			delete pUnit;
			return window;
		}
		pPreUnit = pUnit;
		pUnit = pUnit->next;
	}
	return nullptr;
}


LRESULT CALLBACK WindowProc(HWND _hwnd, UINT _msg, WPARAM _wPalam, LPARAM _lParam)
{
	if (_msg == WM_CLOSE)
		;
	else if (_msg == WM_DESTROY)
		WindowListRemove(_hwnd)->state = false;

	return DefWindowProc(_hwnd, _msg, _wPalam, _lParam);
}



bool Window::InitWindowClass(HINSTANCE _hinstance)
{
	// 声明并填充一个窗口类
	WNDCLASSEX winclass;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;   // 类型
	winclass.lpszClassName = className;
	winclass.lpfnWndProc = WindowProc;  // 消息处理函数
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hinstance;
	winclass.hIcon = nullptr;   // LoadIcon(NULL, APP_ICON);
	winclass.hIconSm = nullptr; // LoadIcon(NULL, APP_ICONSM);
	winclass.hCursor = nullptr; // LoadCursor(NULL, APP_CURSOR);
	winclass.hbrBackground = nullptr;
	winclass.lpszMenuName = nullptr;

	// 注册该窗口类
	if (!RegisterClassEx(&winclass))
		return false;

	hinstance = winclass.hInstance;

	stateClass = true;
	return true;
}


Window::Window()
{
	state = false;

	hwnd = nullptr;

	QuickSetting(WINDOW_MODE_DEFAULT);
}

Window::~Window()
{
	Close();
}


bool Window::QuickSetting(int _mode)
{
	if (state)
		return false;

	switch (_mode)
	{
	case WINDOW_MODE_DEFAULT:

		posx = 0;
		posy = 0;

		width = 800;
		height = 600;

		windowed = true;
		topmost = false;

		break;
	case WINDOW_MODE_GAME_FULLSCREEN:

		posx = 0;
		posy = 0;

		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);

		windowed = false;
		topmost = false;

		break;
	case WINDOW_MODE_GAME_WINDOWED:

		posx = 0;
		posy = 0;

		width = 1024;
		height = 576;

		windowed = true;
		topmost = false;

		break;
	default:
		return false;
	}

	return true;
}

#ifdef UNICODE
bool Window::Create(const wchar_t _windowName[])
#else
bool Window::Create(const char _windowName[])
#endif
{
	if (!stateClass)
		return false;
	if (state)
		return false;

	// 创建窗口
	hwnd = CreateWindowEx( 0,      // 扩展类型
		className ,   // 窗口类名
		_windowName,               // 标题
		(windowed ? (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) : WS_POPUP) | WS_VISIBLE,   // 类型
		posx, posy,	  // 位置 x, y
		width + GetSystemMetrics(SM_CXSIZEFRAME) * 2,   // 宽度
		height + GetSystemMetrics(SM_CYSIZEFRAME) * 2,  // 高度
		nullptr,  // 父窗口句柄
		nullptr,  // 菜单句柄
		hinstance,  // 实例句柄
		nullptr); // 额外的创建参数
	if (!hwnd)
		return false;

	// 总在上层显示
	if (topmost)
		::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

	WindowListAdd(this);

	state = true;
	return true;
}

void Window::Close()
{
	if (!state)
		return;

	PostMessage(hwnd, WM_CLOSE, 0, 0);
}


void Window::MessageLoop(int _messageN)
{
	MSG  msg;
	for (; _messageN > 0; _messageN--)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			break;
	}
}


