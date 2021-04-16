#pragma once
#include <d3d11.h>
#include <d2d1.h>

#include "ComPtr.h"
#include "Print.h"

class Graphics
{
private:
	//3D
	float backgroundColor[4] = { 1,1,1,1 };

	D3D11_VIEWPORT viewport;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11Texture2D> dsTexture;
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<IDXGISurface> surface;

	HRESULT CreateDeviceSwapchain(UINT windowWidth, UINT windowHeight, HWND window);
	HRESULT CreateRenderTarget();
	HRESULT CreateDepthStencil(UINT windowWidth, UINT windowHeight);
	void CreateViewport(UINT windowWidth, UINT windowHeight);
	bool InitializeD3D11(UINT windowWidth, UINT windowHeight, HWND window);

	//2D
	ComPtr<ID2D1Factory> factory;
	ComPtr<ID2D1RenderTarget> renderTarget;

	HRESULT CreateFactory();
	HRESULT Create2DRenderTarget(HWND window);
	bool InitializeD2D1(HWND window);
public:
	Graphics() { viewport = {}; };
	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);

	void BeginFrame();
	void EndFrame() { swapChain->Present(0, 0); };

	ID3D11Device& GetDevice()				{ return *this->device.Get(); };
	ID3D11DeviceContext& GetDeviceContext() { return *this->context.Get(); };
	IDXGISwapChain& GetSwapChain()			{ return *this->swapChain.Get(); };
	IDXGISurface& GetSurface()				{ return *this->surface.Get(); };
	
	ID2D1Factory& GetFactory()				{ return *this->factory.Get(); };
	ID2D1RenderTarget& Get2DRenderTarget()	{ return *this->renderTarget.Get(); };
};