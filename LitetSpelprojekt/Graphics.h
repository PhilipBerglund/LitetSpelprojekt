#pragma once
#include "DirectXEssentials.h"

class Graphics
{
private:
	DirectXEssentials core;
public:
	Graphics() = default;
	bool Initialize(UINT windowWidth, UINT windowHeight, HWND window);

	void BeginFrame()	{ core.BeginScene(1, 1, 1); };
	void EndFrame()		{ core.EndScene(); };

	ID3D11Device& GetDevice()				{ return this->core.GetDevice(); };
	ID3D11DeviceContext& GetDeviceContext() { return this->core.GetDeviceContext(); };
	IDXGISwapChain& GetSwapChain()			{ return this->core.GetSwapChain(); };
	IDXGISurface& GetSurface()				{ return this->core.GetSurface(); };
};