#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "CTimer.h"
using namespace DirectX;

const int vertexElementN = 2;
struct Vertex
{
	XMFLOAT4 pos;
	XMFLOAT4 col;
};

struct VS_CONSTANT_BUFFER
{
	XMMATRIX mWorldViewProj;
	XMFLOAT4 aVector;
	float aFloat;
};

class CDevice
{
private:
	HINSTANCE hInstance;
	HWND hWnd;

	int windowWidth;
	int windowHeight;

	CTimer timer;
	long passTime;
	long sumTime;

	WCHAR windowTitle[100];
	int fpsCounter;
	CTimer timerTitleUpdata;

	D3D_FEATURE_LEVEL featureLevel;

	IDXGISwapChain *pSwapChain;
	ID3D11Device *pD3dDevice;
	ID3D11DeviceContext *pContext;

	ID3D11RenderTargetView *pRTV;
	ID3D11DepthStencilView *pDSV;

	ID3D11VertexShader *pVertexShader;
	ID3D11Buffer *pVSConstantBuffer;
	ID3D11PixelShader *pPixelShader;

	ID3D11InputLayout *pInputLayout;

	ID3D11Buffer *pVertexBuffer;
	ID3D11Buffer *pIndexBuffer;


	HRESULT Render();


public:
	CDevice();
	~CDevice();

	BOOL CreateDevice(HINSTANCE _hInstance, HWND _hWnd, int _width, int _height);
	BOOL MainLoop();
	void FreeResource();

};


HRESULT CreateHighFeatureLevelDeviceDefault(UINT _width, UINT _height, HWND _hWnd, D3D_FEATURE_LEVEL &_maxFeatureLevel,
	IDXGISwapChain *&_pSwapChain, ID3D11Device *&_pD3dDevice, ID3D11DeviceContext *&_pContext);

HRESULT CreateRenderTargetViewDefault(ID3D11Device *_pD3dDevice, IDXGISwapChain *_pSwapChain, ID3D11RenderTargetView *&_pRTV);
HRESULT CreateDepthStencilViewDefault(UINT _width, UINT _height, ID3D11Device *_pD3dDevice, ID3D11DepthStencilView *&_pDSV);

HRESULT LoadShaderFromHLSLDefault(LPCWSTR _fileName, LPCSTR _entryPoint, LPCSTR _profile, ID3DBlob *&_pVSBlob);
HRESULT CreateVertexShaderDefault(ID3D11Device *_pD3dDevice, ID3DBlob *_pVSBlob, ID3D11VertexShader *&_pVertexShader);
HRESULT CreatePixelShaderDefault(ID3D11Device *_pD3dDevice, ID3DBlob *_pPSBlob, ID3D11PixelShader *&_pPixelShader);

HRESULT CreateInputLayoutDefault(ID3D11Device *_pD3dDevice, ID3DBlob *_pVSBlob, ID3D11InputLayout *&_pInputLayout);

HRESULT CreateVertexBufferDefault(ID3D11Device *_pD3dDevice, ID3D11Buffer *&_pVertexBuffer);
HRESULT CreateIndexBufferDefault(ID3D11Device *_pD3dDevice, ID3D11Buffer *&_pIndexBuffer);

HRESULT CreateVSConstantBufferDefault(ID3D11Device *pD3dDevice, ID3D11Buffer *&pConstantBuffer);

