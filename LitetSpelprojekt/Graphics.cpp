#include "Graphics.h"

float Graphics::backgroundColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
D3D11_VIEWPORT Graphics::viewport = {};
ComPtr<ID3D11Device> Graphics::device;
ComPtr<ID3D11DeviceContext> Graphics::context;
ComPtr<IDXGISwapChain> Graphics::swapChain;
ComPtr<ID3D11RenderTargetView> Graphics::rtv;
ComPtr<ID3D11Texture2D> Graphics::dsTexture;
ComPtr<ID3D11DepthStencilView> Graphics::dsView;
ComPtr<IDXGISurface> Graphics::surface;
ComPtr<IDWriteFactory> Graphics::writeFactory;
ComPtr<ID2D1Factory> Graphics::factory;
ComPtr<ID2D1RenderTarget> Graphics::renderTarget;

HRESULT Graphics::CreateDeviceSwapchain(UINT windowWidth, UINT windowHeight, HWND window, bool windowed)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
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
	swapChainDesc.Windowed = windowed;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	#ifdef _DEBUG
		flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &context);
	return hr;
}

HRESULT Graphics::CreateRenderTarget()
{
	HRESULT hr;
	ID3D11Texture2D* backBuffer = nullptr;

	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	if FAILED(hr)
	{
		Error("FAILED TO GET BACK BUFFER");
		return hr;
	}

	// QUERY SURFACE FOR 2D USAGE
	hr = backBuffer->QueryInterface<IDXGISurface>(&surface);
	if FAILED(hr)
	{
		Error("FAILED TO QUERY BACK BUFFER INTERFACE");
		return hr;
	}

	// RENDER TARGET VIEW
	hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);

	backBuffer->Release();

	return hr;
}

HRESULT Graphics::CreateDepthStencil(UINT windowWidth, UINT windowHeight)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC textureDesc = {};

	textureDesc.Width = windowWidth;
	textureDesc.Height = windowHeight;
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
		Error("FAILED TO CREATE 2D TEXTURE");
		return hr;
	}

	hr = device->CreateDepthStencilView(dsTexture.Get(), nullptr, &dsView);
	return hr;
}

void Graphics::CreateViewport(UINT windowWidth, UINT windowHeight)
{
	viewport.Width = static_cast<float>(windowWidth);
	viewport.Height = static_cast<float>(windowHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool Graphics::InitializeD3D11(UINT windowWidth, UINT windowHeight, HWND window, bool windowed)
{
	if FAILED(CreateDeviceSwapchain(windowWidth, windowHeight, window, windowed)) 
	{
		Error("FAILED TO CREATE DEVICE AND SWAP CHAIN");
		return false;
	}

	if FAILED(CreateRenderTarget()) 
	{
		Error("FAILED TO CREATE RENDER TARGET");
		return false;
	}

	if FAILED(CreateDepthStencil(windowWidth, windowHeight)) 
	{
		Error("FAILED TO CREATE DEPTH STENCIL");
		return false;
	}

	CreateViewport(windowWidth, windowHeight);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->RSSetViewports(1, &viewport);
	return true;
}

HRESULT Graphics::CreateWriteFactory()
{
	HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &writeFactory);
	return hr;
}

HRESULT Graphics::CreateFactory()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &factory);
	return hr;
}

HRESULT Graphics::Create2DRenderTarget(HWND window)
{
	UINT dpiX;
	UINT dpiY;
	dpiX = dpiY = GetDpiForWindow(window);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
		);

	HRESULT hr = factory->CreateDxgiSurfaceRenderTarget(surface.Get(), props, &renderTarget);
	return hr;
}

bool Graphics::InitializeD2D1(HWND window)
{
	if FAILED(CreateWriteFactory())
	{
		Error("FAILED TO CREATE WRITE FACTORY");
		return false;
	}

	if FAILED(CreateFactory())
	{
		Error("FAILED TO CREATE FACTORY");
		return false;
	}

	if FAILED(Create2DRenderTarget(window))
	{
		Error("FAILED TO CREATE 2D RENDER TARGET");
		return false;
	}

	return true;
}

bool Graphics::Initialize(UINT windowWidth, UINT windowHeight, HWND window, bool windowed)
{
	if (!InitializeD3D11(windowWidth, windowHeight, window, windowed))
	{
		Error("FAILED TO INITIALIZE D3D11");
		return false;
	}

	if (!InitializeD2D1(window))
	{
		Error("FAILED TO INITIALIZE D2D1");
		return false;
	}

	return true;
}

void Graphics::BeginFrame()
{
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsView.Get());
	context->ClearRenderTargetView(rtv.Get(), backgroundColor);
	context->ClearDepthStencilView(dsView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}