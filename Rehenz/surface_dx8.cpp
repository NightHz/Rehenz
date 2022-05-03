#include "surface_dx8.h"
#include <utility>

#define WINDOW_CLASS_NAME TEXT("REHENZ")

namespace Rehenz
{
	SurfaceDx8::SurfaceDx8()
	{
		window = nullptr;
		window_state = false;
		rc_window.bottom = rc_window.top = rc_window.left = rc_window.right = 0;

		dd = nullptr;
		dds_front = nullptr;
		dds_back = nullptr;
	}

	SurfaceDx8::~SurfaceDx8()
	{
		Destroy();
	}

	bool SurfaceDx8::Create(HINSTANCE _hinstance, int _width, int _height, String _title)
	{
		if (window != nullptr)
			return false;

		// create window
		window = new SimpleWindowWithFC(_hinstance, _width, _height, std::move(_title));
		if (!window->CheckWindowState())
		{
			delete window;
			window = nullptr;
			return false;
		}

		HRESULT hr;

		// DDraw stuff begins here
		hr = DirectDrawCreateEx(nullptr, reinterpret_cast<void**>(&dd), IID_IDirectDraw7, nullptr);
		if (FAILED(hr))
			return false;

		// Set cooperative level
		hr = dd->SetCooperativeLevel(window->GetHwnd(), DDSCL_NORMAL);
		if (FAILED(hr))
			return false;

		// Make sure our window does not hang outside of the work area
		RECT rc, rcWork;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
		GetWindowRect(window->GetHwnd(), &rc);
		if (rc.left < rcWork.left)
			rc.left = rcWork.left;
		if (rc.top < rcWork.top)
			rc.top = rcWork.top;
		SetWindowPos(window->GetHwnd(), nullptr, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

		// Create the primary surface
		DDSURFACEDESC2 ddsd;
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		hr = dd->CreateSurface(&ddsd, &dds_front, nullptr);
		if (FAILED(hr))
			return false;

		// Create the backbuffer surface
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = window->GetWidth();
		ddsd.dwHeight = window->GetHeight();
		hr = dd->CreateSurface(&ddsd, &dds_back, nullptr);
		if (FAILED(hr))
			return false;

		// Done with clipper
		LPDIRECTDRAWCLIPPER pClipper;
		hr = dd->CreateClipper(0, &pClipper, nullptr);
		if (FAILED(hr))
			return false;
		hr = pClipper->SetHWnd(0, window->GetHwnd());
		if (FAILED(hr))
		{
			pClipper->Release();
			return false;
		}
		hr = dds_front->SetClipper(pClipper);
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

		hr = dd->CreateClipper(0, &pBackBufferClipper, nullptr);
		if (FAILED(hr))
			return false;
		SetRect(&rectClipper, 0, 0, window->GetWidth(), window->GetHeight());
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
		hr = dds_back->SetClipper(pBackBufferClipper);
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

	void SurfaceDx8::Destroy()
	{
		if (dds_back)
		{
			dds_back->Release();
			dds_back = nullptr;
		}
		if (dds_front)
		{
			dds_front->Release();
			dds_front = nullptr;
		}
		if (dd)
		{
			dd->Release();
			dd = nullptr;
		}
		if (window)
		{
			delete window;
			window = nullptr;
		}
		window_state = false;
	}

	bool SurfaceDx8::FillFromImage(const uint* image)
	{
		if (window == nullptr)
			return false;
		if (!window_state)
			return true;

		HRESULT hr;
		DDSURFACEDESC2 ddsd;
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		hr = dds_back->Lock(0, &ddsd, DDLOCK_SURFACEMEMORYPTR || DDLOCK_WAIT, 0);
		if (FAILED(hr))
			return false;

		int mempitch = ddsd.lPitch;
		UCHAR* bufferDest = (UCHAR*)ddsd.lpSurface;
		const UCHAR* bufferSrc = (const UCHAR*)image;
		size_t src_pitch = window->GetWidth() * sizeof(uint);
		for (int i = 0; i < window->GetHeight(); i++)
		{
			memcpy(bufferDest, bufferSrc, src_pitch);
			bufferDest += mempitch;
			bufferSrc += src_pitch;
		}

		hr = dds_back->Unlock(0);
		if (FAILED(hr))
			return false;

		return true;
	}

	void SurfaceDx8::UpdateBounds(void)
	{
		GetClientRect(window->GetHwnd(), &rc_window);
		ClientToScreen(window->GetHwnd(), (POINT*)&rc_window);
		ClientToScreen(window->GetHwnd(), (POINT*)&rc_window + 1);
	}

	bool SurfaceDx8::Present()
	{
		if (window == nullptr)
			return false;

		window->Present();

		window_state = window->CheckWindowState();
		if (!window_state)
			return true;

		UpdateBounds();

		HRESULT hr;
		while (true)
		{
			hr = dds_front->Blt(&rc_window, dds_back, nullptr, DDBLT_WAIT, NULL);

			if (hr == DDERR_SURFACELOST)
			{
				dds_front->Restore();
				dds_back->Restore();
			}
			else if (hr == DDERR_WASSTILLDRAWING)
				continue;
			else if (FAILED(hr))
				return false;

			return true;
		}
	}
}