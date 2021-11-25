// Window.h
//
// By NightHz
//
// Build a window.
//

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <Windows.h>


#define WINDOW_MODE_DEFAULT           1000
#define WINDOW_MODE_GAME_FULLSCREEN   1001
#define WINDOW_MODE_GAME_WINDOWED     1002


class Window
{
private:
	static bool stateClass;
	static HINSTANCE hinstance;

	bool state;

	HWND hwnd;

	int posx;
	int posy;

	int width;
	int height;

	bool windowed;
	bool topmost;
	

public:
	static bool InitWindowClass(HINSTANCE _hinstance);

	Window();
	~Window();

	bool QuickSetting(int _mode);

#ifdef UNICODE
	bool Create(const wchar_t _windowName[]);
#else
	bool Create(const char _windowName[]);
#endif

	void Close();

	bool IsOpen() { return state; }

	static void MessageLoop(int _messageN = 50);


	static HINSTANCE GetHinstance() { if (stateClass) return hinstance; else return nullptr; }
	HWND GetHwnd() { if (state) return hwnd; else return nullptr; }

	void GetPos(int &_x, int &_y) { _x = posx; _y = posy; }
	void GetSize(int &_width, int &_height) { _width = width; _height = height; }
	bool IsWindowed() { return windowed; }
	bool IsTopmost() { return topmost; }

	bool SetPos(int _x, int _y) {
		if (state) return false; else { posx = _x; posy = _y; return true; }
	}
	bool SetSize(int _width, int _height) {
		if (state) return false; else { width = _width; height = _height; return true; }
	}
	bool SetWindowed(bool _windowed) {
		if (state) return false; else { windowed = _windowed; return true; }
	}
	bool SetTopmost(bool _topmost) {
		if (state) return false; else { topmost = _topmost; return true; }
	}


private:
	friend LRESULT CALLBACK WindowProc(HWND _hwnd, UINT _msg, WPARAM _wPalam, LPARAM _lParam);

};


#endif // !_WINDOW_H_

