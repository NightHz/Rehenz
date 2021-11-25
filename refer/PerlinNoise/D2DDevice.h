// D2DDevice.h
//
// By NightHz
//
// Using d2d
//

#ifndef _D2DDEVICE_H_
#define _D2DDEVICE_H_

#include <d2d1.h>

class D2DDevice
{
private:
	bool state;
	bool drawState;

	HWND hwnd;

	ID2D1Factory * factory;
	ID2D1HwndRenderTarget * hrt;
	

public:
	D2DDevice();
	~D2DDevice();

	bool Create(HWND _hwnd, ID2D1HwndRenderTarget ** _pHrt);
	void Close();

	ID2D1Factory * GetFactory() { return factory; }

	bool Test();
};


#endif // !_D2DDEVICE_H_

