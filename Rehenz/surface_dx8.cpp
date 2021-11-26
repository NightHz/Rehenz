#include "surface_dx8.h"

#define WINDOW_CLASS_NAME TEXT("REHENZ")

namespace Rehenz
{
	bool SurfaceDx8::InitWindowClass(HINSTANCE hinstance)
	{
		static bool finished = false;
		if (finished)
			return true;
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
		finished = true;
		return true;
	}

	SurfaceDx8::SurfaceDx8(void)
	{
		window_state = false;
		hwnd = nullptr;
		pDD = nullptr;
		pDDSF = nullptr;
		pDDSB = nullptr;

		hinstance = nullptr;
		width = height = 0;
		rc_window.bottom = rc_window.top = rc_window.left = rc_window.right = 0;
		title = nullptr;
	}

	SurfaceDx8::~SurfaceDx8(void)
	{
		Destroy();
	}

	void SurfaceDx8::SetTitle(const TCHAR* _title)
	{
		if (title != nullptr)
			free(title);
		title = (TCHAR*)malloc(lstrlen(_title) * sizeof(TCHAR) + sizeof(TCHAR));
		if (title == nullptr)
			return;
		memcpy(title, _title, lstrlen(_title) * sizeof(TCHAR) + sizeof(TCHAR));
		if (hwnd != nullptr)
			SetWindowText(hwnd, title);
	}

	bool SurfaceDx8::Create(HINSTANCE _hinstance, int _width, int _height, const TCHAR* _title)
	{
		hinstance = _hinstance;
		width = _width;
		height = _height;
		SetTitle(_title);

		if (!InitWindowClass(hinstance))
			return false;

		RECT  rc;
		SetRect(&rc, 0, 0, width, height);
		DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
		AdjustWindowRectEx(&rc, style, false, 0);
		hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME, title, style,
			0, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hinstance, nullptr);
		if (!hwnd)
			return false;

		HRESULT hr;

		// DDraw stuff begins here
		hr = DirectDrawCreateEx(nullptr, (void**)&pDD, IID_IDirectDraw7, nullptr);
		if (FAILED(hr))
			return false;

		// Set cooperative level
		hr = pDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
		if (FAILED(hr))
			return false;

		// Make sure our window does not hang outside of the work area
		RECT rcWork;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
		GetWindowRect(hwnd, &rc);
		if (rc.left < rcWork.left) rc.left = rcWork.left;
		if (rc.top < rcWork.top)  rc.top = rcWork.top;
		SetWindowPos(hwnd, nullptr, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

		// Create the primary surface
		DDSURFACEDESC2 ddsd;
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		hr = pDD->CreateSurface(&ddsd, &pDDSF, nullptr);
		if (FAILED(hr))
			return false;

		// Create the backbuffer surface
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = width;
		ddsd.dwHeight = height;
		hr = pDD->CreateSurface(&ddsd, &pDDSB, nullptr);
		if (FAILED(hr))
			return false;

		// Done with clipper
		LPDIRECTDRAWCLIPPER pClipper;
		hr = pDD->CreateClipper(0, &pClipper, nullptr);
		if (FAILED(hr))
			return false;
		hr = pClipper->SetHWnd(0, hwnd);
		if (FAILED(hr))
		{
			pClipper->Release();
			return false;
		}
		hr = pDDSF->SetClipper(pClipper);
		if (FAILED(hr))
		{
			pClipper->Release();
			return false;
		}
		pClipper->Release();

		// Set BackBufferClipper
		LPDIRECTDRAWCLIPPER pBackBufferClipper;
		RECT rectClipper;
		LPRGNDATA regionData;

		hr = pDD->CreateClipper(0, &pBackBufferClipper, nullptr);
		if (FAILED(hr))
			return false;
		SetRect(&rectClipper, 0, 0, width, height);
		regionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));
		if (regionData == NULL)
			return false;
		memcpy(regionData->Buffer, &rectClipper, sizeof(RECT));
		regionData->rdh.dwSize = sizeof(RGNDATAHEADER);
		regionData->rdh.iType = RDH_RECTANGLES;
		regionData->rdh.nCount = 1;
		regionData->rdh.nRgnSize = sizeof(RECT);
		regionData->rdh.rcBound.left = rectClipper.left;
		regionData->rdh.rcBound.top = rectClipper.top;
		regionData->rdh.rcBound.right = rectClipper.right;
		regionData->rdh.rcBound.bottom = rectClipper.bottom;
		hr = pBackBufferClipper->SetClipList(regionData, 0);
		if (FAILED(hr))
		{
			free(regionData);
			pBackBufferClipper->Release();
			return false;
		}
		hr = pDDSB->SetClipper(pBackBufferClipper);
		if (FAILED(hr))
		{
			free(regionData);
			pBackBufferClipper->Release();
			return false;
		}
		free(regionData);
		pBackBufferClipper->Release();

		UpdateBounds();
		window_state = true;

		return true;
	}

	void SurfaceDx8::Destroy(void)
	{
		if (pDDSB)
		{
			pDDSB->Release();
			pDDSB = nullptr;
		}
		if (pDDSF)
		{
			pDDSF->Release();
			pDDSF = nullptr;
		}
		if (pDD)
		{
			pDD->Release();
			pDD = nullptr;
		}
		if (hwnd)
		{
			DestroyWindow(hwnd);
			hwnd = nullptr;
		}
		if (title != nullptr)
		{
			free(title);
			title = nullptr;
		}
		window_state = false;
	}

	bool SurfaceDx8::FillFromImage(const UINT* image)
	{
		if (hwnd == nullptr)
			return false;

		HRESULT hr;
		DDSURFACEDESC2 ddsd;
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		hr = pDDSB->Lock(0, &ddsd, DDLOCK_SURFACEMEMORYPTR || DDLOCK_WAIT, 0);
		if (FAILED(hr))
			return false;

		int mempitch = ddsd.lPitch;
		UCHAR* bufferDest = (UCHAR*)ddsd.lpSurface;
		const UCHAR* bufferSrc = (const UCHAR*)image;
		for (int i = 0; i < height; i++)
		{
			memcpy(bufferDest, bufferSrc, width * sizeof(int));
			bufferDest += mempitch;
			bufferSrc += width * sizeof(int);
		}

		hr = pDDSB->Unlock(0);
		if (FAILED(hr))
			return false;

		return true;
	}

	void SurfaceDx8::UpdateBounds(void)
	{
		GetClientRect(hwnd, &rc_window);
		ClientToScreen(hwnd, (POINT*)&rc_window);
		ClientToScreen(hwnd, (POINT*)&rc_window + 1);
	}

	bool SurfaceDx8::Present()
	{
		if (hwnd == nullptr)
			return false;

		MSG  msg;
		while (true)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
				break;
		}

		HWND hwndFind = nullptr;
		while (true)
		{
			hwndFind = FindWindowEx(nullptr, hwndFind, WINDOW_CLASS_NAME, title);
			if (!hwndFind)
			{
				hwnd = nullptr;
				Destroy();
				window_state = false;
				return true;
			}
			else if (hwndFind == hwnd)
			{
				window_state = true;
				break;
			}
			else
				continue;
		}

		UpdateBounds();

		HRESULT hr;
		while (true)
		{
			hr = pDDSF->Blt(&rc_window, pDDSB, nullptr, DDBLT_WAIT, NULL);

			if (hr == DDERR_SURFACELOST)
			{
				pDDSF->Restore();
				pDDSB->Restore();
			}
			else if (hr == DDERR_WASSTILLDRAWING)
				continue;
			else if (FAILED(hr))
				return false;

			return true;
		}
	}
}