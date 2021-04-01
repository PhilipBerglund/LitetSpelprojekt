#include "DirectXEssentials.h"
#include "D3D11Helper.h"

DirectXEssentials::DirectXEssentials()
{
	this->device = nullptr;
	this->context = nullptr;
	this->swapChain = nullptr;
	this->rtv = nullptr;
	this->dsTexture = nullptr;
	this->dsView = nullptr;
	this->viewport = {};
}

void DirectXEssentials::ShutDown()
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (context)
	{
		context->Release();
		context = nullptr;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}

	if (rtv)
	{
		rtv->Release();
		rtv = nullptr;
	}

	if (dsTexture)
	{
		dsTexture->Release();
		dsTexture = nullptr;
	}

	if (dsView)
	{
		dsView->Release();
		dsView = nullptr;
	}
}

bool DirectXEssentials::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if (!SetupD3D11(windowWidth, windowWidth, window, device, context, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to setup d3d11" << std::endl;
		return false;
	}

	return true;
}

void DirectXEssentials::BeginScene(float r, float g, float b)
{
	float backgroundColor[4] = { r, g, b, 1 };
	context->ClearRenderTargetView(rtv, backgroundColor);
	context->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DirectXEssentials::EndScene()
{
	swapChain->Present(0, 0);
}

ID3D11Device* DirectXEssentials::GetDevice()
{
	return this->device;
}

ID3D11DeviceContext* DirectXEssentials::GetDeviceContext()
{
	return this->context;
}

ID3D11DepthStencilView* DirectXEssentials::GetDepthStencilView()
{
	return this->dsView;
}