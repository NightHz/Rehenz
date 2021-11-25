#include "stdafx.h"
#include "CDevice.h"


Vertex vertices[] =
{
	XMFLOAT4(-0.5f,  0.5f, -0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	XMFLOAT4( 0.5f,  0.5f, -0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),
	XMFLOAT4( 0.5f,  0.5f,  0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	XMFLOAT4(-0.5f,  0.5f,  0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
	XMFLOAT4(-0.5f, -0.5f, -0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
	XMFLOAT4( 0.5f, -0.5f, -0.5f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
	XMFLOAT4( 0.5f, -0.5f,  0.5f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
	XMFLOAT4(-0.5f, -0.5f,  0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
};
const int vertexN = sizeof(vertices) / sizeof(Vertex);

unsigned int indices[] = { 
	3,1,0,  2,1,3,
	0,5,4,  1,5,0,
	3,4,7,  0,4,3,
	1,6,5,  2,6,1,
	2,7,6,  3,7,2,
	6,4,5,  7,4,6
};
const int triN = sizeof(indices) / sizeof(unsigned int) / 3;

VS_CONSTANT_BUFFER vsConstData;


CDevice::CDevice()
{
	pD3dDevice = nullptr;
	pSwapChain = nullptr;
	pContext = nullptr;

	pDSV = nullptr;
	pRTV = nullptr;

	pVertexShader = nullptr;
	pVSConstantBuffer = nullptr;
	pPixelShader = nullptr;

	pInputLayout = nullptr;

	pVertexBuffer = nullptr;
	pIndexBuffer = nullptr;
}


CDevice::~CDevice()
{
	FreeResource();
}


BOOL CDevice::CreateDevice(HINSTANCE _hInstance, HWND _hWnd, int _width, int _height)
{
	hInstance = _hInstance;
	hWnd = _hWnd;
	windowWidth = _width;
	windowHeight = _height;

	HRESULT hr;

	// [C] Device SwapChain Context
	hr = CreateHighFeatureLevelDeviceDefault(windowWidth, windowHeight, hWnd, featureLevel, pSwapChain, pD3dDevice, pContext);
	if (FAILED(hr))
		return FALSE;

	// [C] RenderTargetView DepthStencilView
	hr = CreateRenderTargetViewDefault(pD3dDevice, pSwapChain, pRTV);
	if (FAILED(hr))
	{
		return FALSE;
	}
	hr = CreateDepthStencilViewDefault(windowWidth, windowHeight, pD3dDevice, pDSV);
	if (FAILED(hr))
	{
		return hr;
	}


	// [C] Shader InputLayout
	ID3DBlob *pBlob = nullptr;
	hr = LoadShaderFromHLSLDefault(L"VertexShader.hlsl", "main", "vs_4_0", pBlob);
	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = CreateInputLayoutDefault(pD3dDevice, pBlob, pInputLayout);
	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = CreateVertexShaderDefault(pD3dDevice, pBlob, pVertexShader);
	if (FAILED(hr))
	{
		return FALSE;
	}

	if(pBlob)
		pBlob->Release();
	pBlob = nullptr;

	hr = LoadShaderFromHLSLDefault(L"PixelShader.hlsl", "main", "ps_4_0", pBlob);
	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = CreatePixelShaderDefault(pD3dDevice, pBlob, pPixelShader);
	if (FAILED(hr))
	{
		return FALSE;
	}
	
	if (pBlob)
		pBlob->Release();
	pBlob = nullptr;

	// [C]Buffer of Vertex and Index
	hr = CreateVertexBufferDefault(pD3dDevice, pVertexBuffer);
	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = CreateIndexBufferDefault(pD3dDevice, pIndexBuffer);
	if (FAILED(hr))
	{
		return FALSE;
	}

	// [C] ConstantBuffer of Shader
	hr = CreateVSConstantBufferDefault(pD3dDevice, pVSConstantBuffer);
	if (FAILED(hr))
	{
		return FALSE;
	}


	pContext->OMSetRenderTargets(1, &pRTV, pDSV);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_width;
	vp.Height = (FLOAT)_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);

	pContext->IASetInputLayout(pInputLayout);
	pContext->VSSetShader(pVertexShader, nullptr, 0);
	pContext->VSSetConstantBuffers(0, 1, &pVSConstantBuffer);
	pContext->PSSetShader(pPixelShader, nullptr, 0);

	UINT strides = sizeof(Vertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &strides, &offset);
	pContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// [S] Timer
	timer.Start();

	GetWindowText(hWnd, windowTitle, 100);
	fpsCounter = 0;
	timerTitleUpdata.Start();


	return TRUE;
}

BOOL CDevice::MainLoop()
{
	passTime = timer.GetPassTime(&sumTime);

	if (timerTitleUpdata.GetPassTime(nullptr, false) >= 1000)
	{
		timerTitleUpdata.Updata();
		WCHAR title[100];
		wsprintfW(title, L"%s fps:%d", windowTitle, fpsCounter);
		SetWindowText(hWnd, title);
		fpsCounter = 0;
	}
	else
		fpsCounter++;

	Render();
	pSwapChain->Present(0, 0);

	return TRUE;
}

void CDevice::FreeResource()
{
	if (pVertexBuffer)
		pVertexBuffer->Release();
	pVertexBuffer = nullptr;
	if (pIndexBuffer)
		pIndexBuffer->Release();
	pIndexBuffer = nullptr;
	if (pVSConstantBuffer)
		pVSConstantBuffer->Release();
	pVSConstantBuffer = nullptr;

	if (pInputLayout)
		pInputLayout->Release();
	pInputLayout = nullptr;

	if (pVertexShader)
		pVertexShader->Release();
	pVertexShader = nullptr;
	if (pPixelShader)
		pPixelShader->Release();
	pPixelShader = nullptr;

	if (pDSV)
		pDSV->Release();
	pDSV = nullptr;
	if (pRTV)
		pRTV->Release();
	pRTV = nullptr;

	if (pD3dDevice)
		pD3dDevice->Release();
	pD3dDevice = nullptr;
	if (pSwapChain)
		pSwapChain->Release();
	pSwapChain = nullptr;
	if (pContext)
		pContext->Release();
	pContext = nullptr;
}


HRESULT CDevice::Render()
{
	static float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // RGBA
	pContext->ClearRenderTargetView(pRTV, clearColor);
	pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	XMMATRIX mWorld = XMMatrixRotationY(sumTime / 1000.0f);
	mWorld *= XMMatrixRotationX(sumTime / 4000.0f);
	mWorld *= XMMatrixTranslation(-0.8f, 0.0f, 0.0f);
	XMMATRIX mView = XMMatrixLookAtLH({ 0,0,-2.0f }, { 0,0,0.0f }, { 0,1,0 });
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(XM_PIDIV2, (float)windowWidth / windowHeight, 0.01f, 100.0f);

	vsConstData.mWorldViewProj = XMMatrixTranspose(mWorld*mView*mProj);
	vsConstData.aVector = { 1.0f,1.0f,1.0f,1.0f };
	vsConstData.aFloat = 1.0f;
	pContext->UpdateSubresource(pVSConstantBuffer, 0, nullptr, &vsConstData, 0, 0);

	pContext->DrawIndexed(triN * 3, 0, 0);

	mWorld *= XMMatrixTranslation(1.6f, 0.0f, 0.0f);
	vsConstData.mWorldViewProj = XMMatrixTranspose(mWorld*mView*mProj);
	vsConstData.aFloat = 0.2f;
	pContext->UpdateSubresource(pVSConstantBuffer, 0, nullptr, &vsConstData, 0, 0);

	pContext->DrawIndexed(triN*3, 0, 0);

	return S_OK;
}

// Start // 弃用函数 在头文件中无声明
#if 0

HRESULT CreateAReferenceDevice(UINT windowWidth, UINT windowHeight, HWND hWnd,
	IDXGISwapChain *&_pSwapChain, ID3D11Device *&_pD3dDevice,
	ID3D11DeviceContext *&_pImmediateContext)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = windowWidth;
	sd.BufferDesc.Height = windowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_REFERENCE,
		NULL,
		0,
		&featureLevels,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&_pSwapChain,
		&_pD3dDevice,
		&featureLevel,
		&_pImmediateContext);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT CreateAWarpDevice(UINT windowWidth, UINT windowHeight, HWND hWnd,
	IDXGISwapChain *&_pSwapChain, ID3D11Device *&_pD3dDevice,
	ID3D11DeviceContext *&_pImmediateContext)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = windowWidth;
	sd.BufferDesc.Height = windowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_10_1;

	HRESULT hr = S_OK;
	D3D_FEATURE_LEVEL featureLevel;
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_WARP,
		NULL,
		0,
		&featureLevels,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&_pSwapChain,
		&_pD3dDevice,
		&featureLevel,
		&_pImmediateContext);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

std::vector<IDXGIAdapter*>
EnumerateAdapters()
{
	IDXGIAdapter *pAdapter;
	std::vector<IDXGIAdapter*> vAdapters;
	IDXGIFactory1* pFactory = nullptr;
	HRESULT hr = S_OK;

	IDXGIOutput *pOutput = nullptr;
	UINT numModes = 0;
	DXGI_MODE_DESC *displayModes = nullptr;
	DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory);
	if (FAILED(hr))
		return vAdapters;

	for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		vAdapters.push_back(pAdapter);

		hr = pAdapter->EnumOutputs(0, &pOutput);
		if (FAILED(hr))
		{
			pFactory->Release();
			return vAdapters;
		}
		hr = pOutput->GetDisplayModeList(format, 0, &numModes, nullptr);
		if (FAILED(hr))
		{
			pFactory->Release();
			pOutput->Release();
			return vAdapters;
		}
		displayModes = new DXGI_MODE_DESC[numModes];
		hr = pOutput->GetDisplayModeList(format, 0, &numModes, displayModes);
		if (FAILED(hr))
		{
			pFactory->Release();
			pOutput->Release();
			delete[] displayModes;
			return vAdapters;
		}

		for (UINT i = 0; i < numModes; ++i)
		{
			displayModes[i];
		}

		pOutput->Release();
		delete[] displayModes;
	}

	if (pFactory)
	{
		pFactory->Release();
	}

	return vAdapters;
}

#endif
// End // 


HRESULT CreateHighFeatureLevelDeviceDefault(UINT _width, UINT _height, HWND _hWnd, D3D_FEATURE_LEVEL &_maxFeatureLevel, 
	IDXGISwapChain *&_pSwapChain, ID3D11Device *&_pD3dDevice, ID3D11DeviceContext *&_pContext)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0,D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,D3D_FEATURE_LEVEL_9_2,D3D_FEATURE_LEVEL_9_1 };

	HRESULT hr = S_OK;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&sd,
		&_pSwapChain,
		&_pD3dDevice,
		&_maxFeatureLevel,
		&_pContext);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT CreateRenderTargetViewDefault(ID3D11Device *_pD3dDevice, IDXGISwapChain *_pSwapChain, ID3D11RenderTargetView *&_pRTV)
{
	HRESULT hr;

	ID3D11Texture2D *pBackBuffer = nullptr;

	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = _pD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRTV);
	if (FAILED(hr))
	{
		return hr;
	}

	if (pBackBuffer)
		pBackBuffer->Release();
	pBackBuffer = nullptr;


	return S_OK;
}

HRESULT CreateDepthStencilViewDefault(UINT _width, UINT _height, ID3D11Device *_pD3dDevice, ID3D11DepthStencilView *&_pDSV)
{
	HRESULT hr;

	ID3D11Texture2D *pTexture = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = _width;
	textureDesc.Height = _height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	hr = _pD3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = _pD3dDevice->CreateDepthStencilView(pTexture, &dsvDesc, &_pDSV);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT LoadShaderFromHLSLDefault(LPCWSTR _fileName,LPCSTR _entryPoint, LPCSTR _profile, ID3DBlob *&_pVSBlob)
{
	HRESULT hr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pErrorBlob = nullptr;

	hr = D3DCompileFromFile(_fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_entryPoint, _profile, flags, 0, &_pVSBlob, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr)
		{
			OutputDebugStringA("<*> LoadShaderFromHLSLDefault() : File:\"");
			OutputDebugStringW(_fileName);
			OutputDebugStringA("\" (A error return value) ErrorBlob:");
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob != nullptr)
	{
		OutputDebugStringA("<*> LoadShaderFromHLSLDefault() : File:\"");
		OutputDebugStringW(_fileName);
		OutputDebugStringA("\" (No error return value) ErrorBlob:");
		OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		pErrorBlob->Release();
	}

	return S_OK;
}

HRESULT CreateVertexShaderDefault(ID3D11Device *_pD3dDevice, ID3DBlob *_pVSBlob, ID3D11VertexShader *&_pVertexShader)
{
	HRESULT hr;

	hr = _pD3dDevice->CreateVertexShader(_pVSBlob->GetBufferPointer(),
		_pVSBlob->GetBufferSize(), 0, &_pVertexShader);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CreatePixelShaderDefault(ID3D11Device *_pD3dDevice, ID3DBlob *_pPSBlob, ID3D11PixelShader *&_pPixelShader)
{
	HRESULT hr;

	hr = _pD3dDevice->CreatePixelShader(_pPSBlob->GetBufferPointer(),
		_pPSBlob->GetBufferSize(), 0, &_pPixelShader);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CreateInputLayoutDefault(ID3D11Device *_pD3dDevice, ID3DBlob *_pVSBlob, ID3D11InputLayout *&_pInputLayout)
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC layout[vertexElementN];

	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "COLOR";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = sizeof(XMFLOAT4);
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	hr = _pD3dDevice->CreateInputLayout(layout, vertexElementN,
		_pVSBlob->GetBufferPointer(), _pVSBlob->GetBufferSize(), &_pInputLayout);
	if (FAILED(hr))
		return hr;


	return S_OK;
}

HRESULT CreateVertexBufferDefault(ID3D11Device *_pD3dDevice, ID3D11Buffer *&_pVertexBuffer)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * vertexN;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = _pD3dDevice->CreateBuffer(&bufferDesc, &initData, &_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT CreateIndexBufferDefault(ID3D11Device *_pD3dDevice, ID3D11Buffer *&_pIndexBuffer)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(indices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = indices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = _pD3dDevice->CreateBuffer(&bufferDesc, &initData, &_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT CreateVSConstantBufferDefault(ID3D11Device *pD3dDevice, ID3D11Buffer *&pConstantBuffer)
{
	HRESULT hr;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	hr = pD3dDevice->CreateBuffer(&bufferDesc, nullptr, &pConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}


