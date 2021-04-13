#pragma once
#include <d3d11.h>
#include "ComPtr.h"

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

	bool SetupD3D11(UINT width, UINT height, HWND window);
public:
	DirectXEssentials() = default;

	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);

	void BeginScene(float r, float g, float b);
	void EndScene();

	ID3D11Device& GetDevice();
	ID3D11DeviceContext& GetDeviceContext();
	ID3D11DepthStencilView& GetDepthStencilView();
	IDXGISwapChain& GetSwapChain();
};