// CSurface.h
//
// By NightHz
//
// 建立一个可以进行绘制的表面
// use DX8
// only Windows
//

#pragma once
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")

#include <ddraw.h>
#include <Windows.h>


class CSurface
{
private:
	HINSTANCE hinstance;
	HWND hwnd;

	int width;
	int height;
	RECT rc_window;

	LPDIRECTDRAW7 pDD;
	LPDIRECTDRAWSURFACE7 pDDSF;
	LPDIRECTDRAWSURFACE7 pDDSB;

	void UpdateBounds(void);

public:
	CSurface(void);
	~CSurface(void);

	// Access functions
	HWND GetHWnd(void) { return hwnd; }

	void SetTitle(const TCHAR * title) { SetWindowText(hwnd, title); }

	// Creation / destruction methods
	bool Create(HINSTANCE hinstance, int width, int height, const TCHAR * title);
	void Destroy(void);

	// Draw Method
	bool FillFromImage(UINT * image);

	// Display Method
	bool Present(bool & exitState);

};