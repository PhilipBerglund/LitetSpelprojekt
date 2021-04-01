#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>
#include "WindowHelper.h"
#include "D3D11Helper.h"
#include "PipelineHelper.h"

#include <chrono>

using namespace DirectX;

void Render()
{

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;

	HWND window;


	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Failed to setup window!" << std::endl;
		return -1;
	}

	//DirectX Setup
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;	
	IDXGISwapChain* swapChain;			
	ID3D11RenderTargetView* rtv;		
	ID3D11Texture2D* dsTexture;			
	ID3D11DepthStencilView* dsView;		
	D3D11_VIEWPORT viewport;			

	//För Quaden
	ID3D11InputLayout* inputLayout;			
	ID3D11VertexShader* vShader;		
	ID3D11PixelShader* pShader;			

	ID3D11RasterizerState* rastState;	

	ID3D11SamplerState* sampler;



	//Funktion för att "initiera" D3D11 (D3D11Helper.h)
	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to setup d3d11!" << std::endl;
		return -1;
	}

	//Funktion som sätter upp pipe-line stegen
	if (!SetupPipeline(device, vShader, pShader, inputLayout, sampler))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	//Funktion som sätter upp Rasterize-steget
	if (!SetupRasterizerState(device, rastState))
	{
		std::cerr << "Failed to setup Rasterizer-state!" << std::endl;
		return -1;
	}

	MSG msg = {};

	while (msg.message != WM_QUIT)
	{	

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Render();
		swapChain->Present(1, 0);
	}

	sampler->Release();
	rastState->Release();
	inputLayout->Release();
	pShader->Release();
	vShader->Release();
	dsView->Release();
	dsTexture->Release();
	rtv->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();

	return 0;
}


ID3D11Buffer* createConstantBuffer(UINT size, D3D11_SUBRESOURCE_DATA* pData, ID3D11Device* device, bool cpuWrite)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.MiscFlags = 0; 
	desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	desc.Usage = D3D11_USAGE_DYNAMIC;

	if (cpuWrite == true)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		desc.CPUAccessFlags = 0;
	}

	ID3D11Buffer* pBuffer = 0;
	HRESULT hr = device->CreateBuffer(&desc, pData, &pBuffer);

	if (FAILED(hr))
	{
		std::cerr << "Failed to create constantBuffer!" << std::endl;
		return (0);
	}
	return(pBuffer);
}
