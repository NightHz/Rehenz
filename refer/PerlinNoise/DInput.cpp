// DInput.h
//
// By NightHz
//

#define WIN32_LEAN_AND_MEAN

#define _NDEBUG_INFO

#include "DInput.h"
#ifdef _DEBUG_INFO
#include <stdio.h>
#endif

#pragma comment(lib, "Dinput8.lib")
#pragma comment(lib, "dxguid.lib")



DInput * DInput::instance = nullptr;

DInput * DInput::GetInstance()
{
	if (!instance)
		instance = new DInput;
	return instance;
}


DInput::DInput()
{
	state = false;
}

DInput::~DInput()
{
	Close();
}

bool DInput::Create(HINSTANCE _hInstance, HWND _hwnd)
{
	if (state)
		return false;

	HRESULT hr;

	hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&di, nullptr);
	if (FAILED(hr))
	{
#ifdef _DEBUG_INFO
		printf("Create DirectInput8 Failed \n");
#endif
		return false;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	if (FAILED(hr))
	{
#ifdef _DEBUG_INFO
		printf("Create Keyboard Device Failed \n");
#endif
		di->Release();
		return false;
	}

	if (_hwnd)
	{
		hr = keyboard->SetCooperativeLevel(_hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr))
		{
#ifdef _DEBUG_INFO
			printf("Set Keyboard Cooperative Level Failed \n");
#endif
			keyboard->Release();
			di->Release();
			return false;
		}
	}

	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
#ifdef _DEBUG_INFO
		printf("Set Keyboard Data Format Failed \n");
#endif
		keyboard->Release();
		di->Release();
		return false;
	}

	hr = keyboard->Acquire();
	if (FAILED(hr))
	{
#ifdef _DEBUG_INFO
		printf("Acquire Keyboard Failed \n");
#endif
		keyboard->Release();
		di->Release();
		return false;
	}

	state = true;
	return true;
}

void DInput::Close()
{
	if (!state)
		return;

	keyboard->Unacquire();
	keyboard->Release();
	di->Release();

	state = false;
}

bool DInput::UpdateKeyboardState()
{
	if (!state)
		return false;

	HRESULT hr;

	hr = keyboard->GetDeviceState(sizeof(keyboardState), keyboardState);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool DInput::GetKeyboardState(UCHAR _keyID)
{
	if (!state)
		return false;

	return (keyboardState[_keyID] & 0x80);
}

bool DInput::GetKeyboardState(UCHAR(*_keyState)[32])
{
	if (!state)
		return false;

	int i,j,t;
	for (t = 0; t < 32; t++)
	{
		(*_keyState)[t] = 0;
	}
	for (i = 0, j = 0, t = 0; i < 256; i++, j++)
	{
		if (j >= 8)
		{
			j -= 8;
			t++;
		}
		if((keyboardState[i] & 0x80))
			(*_keyState)[t] |= (0x80 >> j);
	}

	return true;
}

