#pragma once
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>

#include "ComPtr.h"
#include "Print.h"

class Graphics
{
private:
	//3D
	static float backgroundColor[4];

	static D3D11_VIEWPORT viewport;
	static ComPtr<ID3D11Device> device;
	static ComPtr<ID3D11DeviceContext> context;
	static ComPtr<IDXGISwapChain> swapChain;
	static ComPtr<ID3D11RenderTargetView> rtv;
	static ComPtr<ID3D11Texture2D> dsTexture;
	static ComPtr<ID3D11DepthStencilView> dsView;
	static ComPtr<IDXGISurface> surface;
	static ComPtr<ID3D11SamplerState> wrapSampler;

	static HRESULT CreateDeviceSwapchain(UINT windowWidth, UINT windowHeight, HWND window, bool windowed);
	static HRESULT CreateRenderTarget();
	static HRESULT CreateDepthStencil(UINT windowWidth, UINT windowHeight);
	static HRESULT CreateWrapSamplerState();
	static void CreateViewport(UINT windowWidth, UINT windowHeight);
	static bool InitializeD3D11(UINT windowWidth, UINT windowHeight, HWND window, bool windowed);

	//2D
	static ComPtr<IDWriteFactory> writeFactory;
	static ComPtr<ID2D1Factory> factory;
	static ComPtr<ID2D1RenderTarget> renderTarget;
	 
	static HRESULT CreateWriteFactory();
	static HRESULT CreateFactory();
	static HRESULT Create2DRenderTarget(HWND window);
	static bool InitializeD2D1(HWND window);
public:
	static bool Initialize(UINT windowWidth, UINT windowHeight, HWND window, bool windowed);
	static void BeginFrame();
	static void EndFrame() { swapChain->Present(0, 0); };

	static ID3D11Device& GetDevice()				{ return *device.Get(); };
	static ID3D11DeviceContext& GetDeviceContext()	{ return *context.Get(); };
	static ID3D11RenderTargetView** GetRenderTargetView() { return rtv.GetAddressOf(); };
	static ID3D11DepthStencilView* GetDepthStencilView() { return dsView.Get(); };
	//static D3D11_VIEWPORT& GetViewport() { return viewport; };
	static IDXGISwapChain& GetSwapChain()			{ return *swapChain.Get(); };
	static IDXGISurface& GetSurface()				{ return *surface.Get(); };
	static ID3D11SamplerState** GetWrapSampler()	{ return wrapSampler.GetAddressOf(); }
	
	static IDWriteFactory& GetWriteFactory()		{ return *writeFactory.Get(); };
	static ID2D1Factory& GetFactory()				{ return *factory.Get(); };
	static ID2D1RenderTarget& Get2DRenderTarget()	{ return *renderTarget.Get(); };
};