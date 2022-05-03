// the main content is written before 2018, and move it here
// modify in 2022
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <ddraw.h>
#include <Windows.h>
#include "window_fc.h"
#include "type.h"

namespace Rehenz
{
	// require link ddraw.lib and dxguid.lib
	class SurfaceDx8
	{
	private:
		SimpleWindowWithFC* window;
		bool window_state;
		RECT rc_window;

		IDirectDraw7* dd;
		IDirectDrawSurface7* dds_front;
		IDirectDrawSurface7* dds_back;

		void UpdateBounds();

	public:
		SurfaceDx8();
		SurfaceDx8(const SurfaceDx8&) = delete;
		SurfaceDx8& operator=(const SurfaceDx8&) = delete;
		~SurfaceDx8();

		// Access functions
		inline SimpleWindowWithFC* GetWindow() { return window; }
		inline bool GetWindowState() { return window_state; }

		// Creation / destruction methods
		bool Create(HINSTANCE _hinstance, int _width, int _height, String _title);
		void Destroy();

		// Draw Method
		bool FillFromImage(const uint* image);

		// Display Method
		bool Present();

	};
}