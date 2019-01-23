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

namespace Rehenz
{
	namespace Windows
	{
		class CSurface
		{
		private:
			HINSTANCE hinstance;
			HWND hwnd;

			int width;
			int height;
			RECT rc_window;
			const TCHAR * title;

			LPDIRECTDRAW7 pDD;
			LPDIRECTDRAWSURFACE7 pDDSF;
			LPDIRECTDRAWSURFACE7 pDDSB;

			void UpdateBounds(void);

			static bool InitWindowClass(HINSTANCE hinstance);

		public:
			CSurface(void);
			~CSurface(void);

			// Access functions
			HWND GetHWnd(void) { return hwnd; }

			void SetTitle(const TCHAR * title) { SetWindowText(hwnd, title); this->title = title; }

			// Creation / destruction methods
			bool Create(HINSTANCE hinstance, int width, int height, const TCHAR * title);
			void Destroy(void);

			// Draw Method
			bool FillFromImage(UINT * image);

			// Display Method
			bool Present(bool & windowState);

		};
	}
}