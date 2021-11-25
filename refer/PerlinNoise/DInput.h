// DInput.h
//
// By NightHz
//
// Use DirectInput to get input.
//

#ifndef _DINPUT_H_
#define _DINPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>


class DInput
{
private:
	static DInput * instance;

	bool state;

	IDirectInput8 * di;

	IDirectInputDevice8 * keyboard;
	UCHAR keyboardState[256];

public:
	static DInput * GetInstance();

	bool Create(HINSTANCE _hInstance, HWND _hwnd);
	void Close();

	bool UpdateKeyboardState();

	bool GetKeyboardState(UCHAR _keyID);
	bool GetKeyboardState(UCHAR(*_keyState)[32]);

private:
	DInput();
	~DInput();

};



#endif // !_DINPUT_H_

