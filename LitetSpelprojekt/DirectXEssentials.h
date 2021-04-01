#pragma once
#include <d3d11.h>

class DirectXEssentials
{
private:
	D3D11_VIEWPORT viewport;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
public:
	DirectXEssentials();

	void ShutDown();
	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);

	void BeginScene(float r, float g, float b);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11DepthStencilView* GetDepthStencilView();
};