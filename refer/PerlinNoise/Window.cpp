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
	// ���������һ��������
	WNDCLASSEX winclass;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;   // ����
	winclass.lpszClassName = className;
	winclass.lpfnWndProc = WindowProc;  // ��Ϣ������
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hinstance;
	winclass.hIcon = nullptr;   // LoadIcon(NULL, APP_ICON);
	winclass.hIconSm = nullptr; // LoadIcon(NULL, APP_ICONSM);
	winclass.hCursor = nullptr; // LoadCursor(NULL, APP_CURSOR);
	winclass.hbrBackground = nullptr;
	winclass.lpszMenuName = nullptr;

	// ע��ô�����
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

	// ��������
	hwnd = CreateWindowEx( 0,      // ��չ����
		className ,   // ��������
		_windowName,               // ����
		(windowed ? (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX) : WS_POPUP) | WS_VISIBLE,   // ����
		posx, posy,	  // λ�� x, y
		width + GetSystemMetrics(SM_CXSIZEFRAME) * 2,   // ���
		height + GetSystemMetrics(SM_CYSIZEFRAME) * 2,  // �߶�
		nullptr,  // �����ھ��
		nullptr,  // �˵����
		hinstance,  // ʵ�����
		nullptr); // ����Ĵ�������
	if (!hwnd)
		return false;

	// �����ϲ���ʾ
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


