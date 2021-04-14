#include "DirectXEssentials.h"
#include <iostream>

bool DirectXEssentials::SetupD3D11(UINT width, UINT height, HWND window)
{
	HRESULT hr;

	// DEVICE SWAPCHAIN CONTEXT
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	if (_DEBUG)
	{
		flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;
	}

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &context);

	if FAILED(hr)
	{
		Error("- FAILED TO CREATE DEVICE / SWAP CHAIN / CONTEXT -");
		return false;
	}

	// BACK BUFFER
	ID3D11Texture2D* backBuffer = nullptr;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	if FAILED(hr)		//Pointer till den nuvarande aktiva buffern i swapchainen, eftersom vi har DXGI_SWAP_EFFECT_DISCARD kan vi endast använda den buffer som är på plats 0
	{
		Error("- FAILED TO GET BACK BUFFER -");
		return false;
	}

	// QUERY SURFACE FOR 2D USAGE
	hr = backBuffer->QueryInterface<IDXGISurface>(&surface);
	if FAILED(hr)
	{
		Error("- FAILED TO QUERY BACK BUFFER INTERFACE -");
		return false;
	}

	// RENDER TARGET VIEW
	hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);
	if FAILED(hr)
	{
		Error("- FAILED TO CREATE RENDER TARGET VIEW - ");
		return false;
	}

	backBuffer->Release();

	// DEPTH STENCIL
	D3D11_TEXTURE2D_DESC textureDesc = {};

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&textureDesc, nullptr, &dsTexture);
	if FAILED(hr)
	{
		Error("- FAILED TO CREATE 2D TEXTURE -");
		return false;
	}

	hr = device->CreateDepthStencilView(dsTexture.Get(), nullptr, &dsView);
	if FAILED(hr)
	{
		Error("- FAILED TO CREATE DEPTH STENCIL VIEW -");
		return false;
	}

	// VIEWPORT
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	return true;
}

bool DirectXEssentials::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if (!SetupD3D11(windowWidth, windowWidth, window))
	{
		Error("- FAILED TO SETUP D3D11 -");
		return false;
	}

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->RSSetViewports(1, &viewport);

	return true;
}

void DirectXEssentials::BeginScene(float r, float g, float b)
{
	float backgroundColor[4] = { r, g, b, 1 };
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsView.Get());
	context->ClearRenderTargetView(rtv.Get(), backgroundColor);
	context->ClearDepthStencilView(dsView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}