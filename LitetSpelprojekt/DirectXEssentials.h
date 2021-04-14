#pragma once
#include <d3d11.h>
#include "ComPtr.h"
#include "Print.h"

class DirectXEssentials
{
private:
	D3D11_VIEWPORT viewport;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11Texture2D> dsTexture;
	ComPtr<ID3D11DepthStencilView> dsView;
	ComPtr<IDXGISurface> surface;

	bool SetupD3D11(UINT width, UINT height, HWND window);
public:
	DirectXEssentials() = default;

	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);

	void BeginScene(float r, float g, float b);
	void EndScene() { swapChain->Present(0, 0); };

	ID3D11Device& GetDevice()						{ return *this->device.Get(); };
	ID3D11DeviceContext& GetDeviceContext()			{ return *this->context.Get(); };
	ID3D11DepthStencilView& GetDepthStencilView()	{ return *this->dsView.Get(); };
	IDXGISwapChain& GetSwapChain()					{ return *this->swapChain.Get(); };
	IDXGISurface& GetSurface()						{ return *this->surface.Get(); };
};