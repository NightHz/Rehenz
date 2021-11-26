// the main content is written before 2018, and move it here
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <ddraw.h>
#include <Windows.h>

namespace Rehenz
{
	// require link ddraw.lib and dxguid.lib
	class SurfaceDx8
	{
	private:
		HINSTANCE hinstance;
		HWND hwnd;

		bool window_state;
		int width;
		int height;
		RECT rc_window;
		TCHAR* title;

		LPDIRECTDRAW7 pDD;
		LPDIRECTDRAWSURFACE7 pDDSF;
		LPDIRECTDRAWSURFACE7 pDDSB;

		void UpdateBounds(void);

		static bool InitWindowClass(HINSTANCE hinstance);

	public:
		SurfaceDx8(void);
		~SurfaceDx8(void);

		// Access functions
		HWND GetHWnd(void) { return hwnd; }
		bool GetWindowState() { return window_state; }
		const TCHAR* GetTitle() { return title; }

		void SetTitle(const TCHAR* _title);

		// Creation / destruction methods
		bool Create(HINSTANCE _hinstance, int _width, int _height, const TCHAR* _title);
		void Destroy(void);

		// Draw Method
		bool FillFromImage(const UINT* image);

		// Display Method
		bool Present();

	};

	// letter and number are the same as ASCII code
	inline bool KeyIsDown(int vKey)
	{
		return (GetAsyncKeyState(vKey) & 0x8000);
	}

	// letter and number are the same as ASCII code
	inline bool KeyIsUp(int vKey)
	{
		return !KeyIsDown(vKey);
	}
}