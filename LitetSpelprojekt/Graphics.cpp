#include "Graphics.h"
#include "PipelineHelper.h"

bool Graphics::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if (!core.Initialize(windowWidth, windowHeight, window))
		return false;

	return true;
}

void Graphics::BeginFrame()
{
	core.BeginScene(1, 1, 1);
}

void Graphics::EndFrame()
{
	core.EndScene();
}

ID3D11Device& Graphics::GetDevice()
{
	return core.GetDevice();
}

ID3D11DeviceContext& Graphics::GetDeviceContext()
{
	return core.GetDeviceContext();
}