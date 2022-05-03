#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "type.h"

namespace Rehenz
{
	// require link user32.lib
	class SimpleWindow
	{
	private:
		const HINSTANCE hinstance;
		HWND hwnd;
		const int width, height;
		String title;

		static bool InitDefaultWindowClass(HINSTANCE hinstance);

	public:
		SimpleWindow(HINSTANCE _hinstance, int _width, int _height, String _title);
		SimpleWindow(const SimpleWindow&) = delete;
		SimpleWindow& operator=(const SimpleWindow&) = delete;
		~SimpleWindow();

		HINSTANCE GetHinstance() { return hinstance; }
		HWND GetHwnd() { return hwnd; }
		int GetWidth() { return width; }
		int GetHeight() { return height; }
		const String& GetTitle() { return title; }

		bool CheckWindowState();
		void SetTitle(String _title);
	};

	void SimpleMessageProcess();
}