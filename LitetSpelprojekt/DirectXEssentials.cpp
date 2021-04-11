#include "DirectXEssentials.h"
#include <iostream>

bool DirectXEssentials::SetupD3D11(UINT width, UINT height, HWND window)
{
	// DEVICE SWAPCHAIN CONTEXT
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};	//Initiera en tom swapChain description

	//Definiera innre innehåll i en buffer som ligger i SwapChain
	swapChainDesc.BufferDesc.Width = width;		//Storlek på fönstret
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//8 bitar för varje kanal, unsigned normaliserad, används internt för att skapa värden från 0-1 istället för 0 till 255 som man annars kunnat göra med 8 bitar
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //Default scan-line
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //Default skalning

	swapChainDesc.SampleDesc.Count = 1; //Default, 1 sample-desc
	swapChainDesc.SampleDesc.Quality = 0;	//Default

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//Vi använder denna som render target, som vi kan rita till
	swapChainDesc.BufferCount = 1; //1 buffer
	swapChainDesc.OutputWindow = window;	//vi kopplar swapchainen till vårt fönster
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//Default hur den byter mellan front och back-buffer
	swapChainDesc.Flags = 0;	//Inga extra flaggor, går att göra speciella ändringar beroende på vad man vill göra med sin swapchain

	UINT flags = 0;
	if (_DEBUG)		//Macro: Kommer att vara true om vi compilar i Debug, ger oss extra information i Debug-läge endast
	{
		//Annars sätter vi flaggan till nedan
		flags = D3D11_CREATE_DEVICE_DEBUG;
	}
	//Array då vi kan använda flera (inte relevant i vårt exempel)
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };		//Ett sätt att säga åt vilken feature level vi vill använda. Vi vill använda all funktionalitet för D3D11

	//Resultatet av operationen sparar vi i en HRESULT
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &context);
	//Default grafikkort,   vilken drivartyp,	går att specifiera drivar-software,    vi skickar med flaggorna,   vilken feature level,   hur många värden det finns i FL-arrayen,  SDK-version,   vår swapchain-beskrivning,  vår swapchain, device,	går att stoppa med FL-pekare för att få reda på vilken som används,  vår immediateContext

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (FAILED(hr))
		return false;


	// RENDER TARGET VIEW
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))		//Pointer till den nuvarande aktiva buffern i swapchainen, eftersom vi har DXGI_SWAP_EFFECT_DISCARD kan vi endast använda den buffer som är på plats 0
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);
	backBuffer->Release();
	if (FAILED(hr))
		return false;


	// DEPTH STENCIL
	D3D11_TEXTURE2D_DESC textureDesc;

	textureDesc.Width = width;		//Sätts till storleken på fönstret
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;		//Mindre färdig-komprimerade versioner av sig själv, vi behöver bara 1
	textureDesc.ArraySize = 1;		//1 Buffer
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		//24 bitar till djupet, unsigned normaliserad (0-1) - Bör följa ett format som matchar texturen. GPU'n jobbar i flyttal med depth-stencil från 0-1, därför normaliserad.
	textureDesc.SampleDesc.Count = 1;		//Default (1 styck)
	textureDesc.SampleDesc.Quality = 0;		//Default
	textureDesc.Usage = D3D11_USAGE_DEFAULT;	//Default
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//Binder som en depth-stencil då den används som det
	textureDesc.CPUAccessFlags = 0;	//CPU'n behöver inte läsa eller skriva
	textureDesc.MiscFlags = 0;		//Behövs inga flaggor


	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		std::cerr << "Failed to create depth stencil texture!" << std::endl;
		return false;
	}

	hr = device->CreateDepthStencilView(dsTexture.Get(), nullptr, &dsView);
	if (FAILED(hr))
		return false;

	// VIEWPORT
	//Definierar boundaries för vår viewport (Vart vi ritar)
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool DirectXEssentials::Initialize(UINT windowWidth, UINT windowHeight, HWND window)
{
	if (!SetupD3D11(windowWidth, windowWidth, window))
	{
		std::cerr << "Failed to setup d3d11" << std::endl;
		return false;
	}

	context->RSSetViewports(1, &viewport);

	return true;
}

void DirectXEssentials::BeginScene(float r, float g, float b)
{
	float backgroundColor[4] = { r, g, b, 1 };
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsView.Get());
	context->ClearRenderTargetView(rtv.Get(), backgroundColor);
	context->ClearDepthStencilView(dsView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DirectXEssentials::EndScene()
{
	swapChain->Present(0, 0);
}

ID3D11Device& DirectXEssentials::GetDevice()
{
	return *this->device.Get();
}

ID3D11DeviceContext& DirectXEssentials::GetDeviceContext()
{
	return *this->context.Get();
}

ID3D11DepthStencilView& DirectXEssentials::GetDepthStencilView()
{
	return *this->dsView.Get();
}